// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/libmevent/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef libmevent_NET_TIMERID_H
#define libmevent_NET_TIMERID_H

#include "libmevent/base/copyable.h"

namespace libmevent
{
namespace net
{

    class Timer;

    ///
    /// An opaque identifier, for canceling Timer.
    ///
    class TimerId : public libmevent::copyable
    {
    public:
        TimerId() : timer_(NULL), sequence_(0)
        {
        }

        TimerId(Timer* timer, int64_t seq) : timer_(timer), sequence_(seq)
        {
        }

        // default copy-ctor, dtor and assignment are okay

        friend class TimerQueue;

    private:
        Timer* timer_;
        int64_t sequence_;
    };

}  // namespace net
}  // namespace libmevent

#endif  // libmevent_NET_TIMERID_H
