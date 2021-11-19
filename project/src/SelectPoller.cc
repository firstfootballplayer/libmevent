// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/libmevent/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "libmevent/net/SelectPoller.h"

#include "libmevent/base/Logging.h"
#include "libmevent/base/Types.h"
#include "libmevent/net/Channel.h"

#include <assert.h>
#include <errno.h>
#include <poll.h>

using namespace libmevent;
using namespace libmevent::net;

SelectPoller::SelectPoller(EventLoop* loop) : Poller(loop)
{
    printf("SelectPoller\n");
    FD_ZERO(&o_readset_);
    FD_ZERO(&w_readset_);
    FD_ZERO(&o_writeset_);
    maxi_ = -1; /* index into client[] array */
    maxfd_ = -1;
    for(int i = 0; i < FD_SETSIZE; i++)
        client_[i] = -1;
}

SelectPoller::~SelectPoller() = default;

Timestamp SelectPoller::poll(int timeoutMs, ChannelList* activeChannels)
{
    w_readset_ = o_readset_;
    w_writeset_ = o_writeset_;
    int numEvents = ::select(maxfd_ + 1, &w_readset_, &w_writeset_, NULL, NULL);
    int savedErrno = errno;
    // printf("ret %d,maxfd %d\n", numEvents, maxfd_);
    Timestamp now(Timestamp::now());
    if(numEvents > 0) {
        LOG_TRACE << numEvents << " events happened";
        fillActiveChannels(numEvents, activeChannels);
    } else if(numEvents == 0) {
        LOG_TRACE << " nothing happened";
    } else {
        if(savedErrno != EINTR) {
            errno = savedErrno;
            LOG_SYSERR << "SelectPoller::poll()";
        }
    }
    return now;
}

void SelectPoller::fillActiveChannels(int numEvents, ChannelList* activeChannels) const
{
    int sockfd;
    for(int i = 0; i <= maxi_; i++) {
        if((sockfd = client_[i]) < 0)
            continue;
        ChannelMap::const_iterator ch = channels_.find(sockfd);
        assert(ch != channels_.end());
        Channel* channel = ch->second;
        // printf("Channelfd %d:sockfd %d:idx %d\n", channel->fd(), sockfd, i);
        assert(channel->fd() == sockfd);
        int revents = 0;
        if(FD_ISSET(sockfd, &w_readset_)) {
            revents |= kReadEvent;
            channel->set_revents(revents);
            activeChannels->push_back(channel);
            if(--numEvents <= 0)
                break;
        }
        if(FD_ISSET(sockfd, &w_writeset_)) {
            revents |= kWriteEvent;
            channel->set_revents(revents);
            activeChannels->push_back(channel);
            if(--numEvents <= 0)
                break;
        }
    }
}

void SelectPoller::updateChannel(Channel* channel)
{
    Poller::assertInLoopThread();
    LOG_TRACE << "fd = " << channel->fd() << " events = " << channel->events();
    if(channel->index() < 0) {
        assert(channels_.find(channel->fd()) == channels_.end());
        int idx;
        for(idx = 0; idx < FD_SETSIZE; idx++)
            if(client_[idx] < 0) {
                client_[idx] = channel->fd(); /* save descriptor */
                break;
            }
        if(idx == FD_SETSIZE) {
            LOG_SYSERR << "too many clients";
            return;
        }
        channel->set_index(idx);
        channels_[client_[idx]] = channel;
        if(channel->fd() > maxfd_)
            maxfd_ = channel->fd(); /* for select */
        if(idx > maxi_)
            maxi_ = idx;
        FD_SET(channel->fd(), &o_readset_);
    } else {
        // update existing one
        assert(channels_.find(channel->fd()) != channels_.end());
        assert(channels_[channel->fd()] == channel);
        int idx = channel->index();
        assert(0 <= idx && idx < static_cast<int>(FD_SETSIZE));
        assert(client_[idx] == channel->fd() || client_[idx] == -channel->fd() - 1);
        client_[idx] = channel->fd();
        if(channel->isReading())
            FD_SET(channel->fd(), &o_readset_);
        else {
            // printf("pause\n");
            // pause();
            // LOG_INFO << libmevent::CurrentThread::stackTrace(true).c_str();
            FD_CLR(channel->fd(), &o_readset_);
        }
        if(channel->isWriting())
            FD_SET(channel->fd(), &o_writeset_);
        else
            FD_CLR(channel->fd(), &o_writeset_);
        if(channel->isNoneEvent()) {
            // ignore this pollfd
            client_[idx] = -channel->fd() - 1;
        }
    }
}

void SelectPoller::removeChannel(Channel* channel)
{
    // printf("removeChannel\n");

    Poller::assertInLoopThread();
    // LOG_TRACE << "fd = " << channel->fd();
    assert(channels_.find(channel->fd()) != channels_.end());
    assert(channels_[channel->fd()] == channel);
    assert(channel->isNoneEvent());
    int idx = channel->index();
    assert(0 <= idx && idx < static_cast<int>(FD_SETSIZE));
    size_t n = channels_.erase(channel->fd());
    assert(n == 1);
    client_[idx] = -1;
    FD_CLR(channel->fd(), &o_readset_);
    FD_CLR(channel->fd(), &o_writeset_);
    if(channel->fd() == maxfd_)  // 需要更新max_fd_;
    {
        int fd = 0;
        for(int i = 0; i < FD_SETSIZE; i++) {
            if(client_[i] == -1 || client_[i] == maxfd_)
                continue;
            if(client_[i] > fd)
                fd = client_[i];
        }
        maxfd_ = fd;
    }
    client_[idx] = -1;
}
