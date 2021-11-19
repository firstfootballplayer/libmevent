// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/libmevent/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "libmevent/net/Poller.h"
#include "libmevent/net/PollPoller.h"
#include "libmevent/net/EPollPoller.h"
#include "libmevent/net/SelectPoller.h"
#include <stdlib.h>

using namespace libmevent::net;

Poller* Poller::newDefaultPoller(EventLoop* loop)
{
    if(::getenv("libmevent_USE_POLL")) {
        return new PollPoller(loop);
    } else {
        // return new EPollPoller(loop);
        // return new SelectPoller(loop);
        return new PollPoller(loop);
    }
}
