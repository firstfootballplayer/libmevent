// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/libmevent/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef libmevent_NET_POLLER_SelectPoller_H
#define libmevent_NET_POLLER_SelectPoller_H

#include "libmevent/net/Poller.h"
#include <vector>
#include <poll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
namespace libmevent
{
namespace net
{

    ///
    /// IO Multiplexing with epoll(4).
    ///
    class SelectPoller : public Poller
    {
    public:
        SelectPoller(EventLoop* loop);
        ~SelectPoller() override;

        Timestamp poll(int timeoutMs, ChannelList* activeChannels) override;
        void updateChannel(Channel* channel) override;
        void removeChannel(Channel* channel) override;

    private:
        void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;
        const int kReadEvent = POLLIN | POLLPRI;
        const int kWriteEvent = POLLOUT;
        int nready_, maxi_, maxfd_, client_[FD_SETSIZE];
        fd_set o_readset_, w_readset_, o_writeset_, w_writeset_;
    };

}  // namespace net
}  // namespace libmevent
#endif  // libmevent_NET_POLLER_SelectPoller_H
