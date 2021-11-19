// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef libmevent_BASE_COUNTDOWNLATCH_H
#define libmevent_BASE_COUNTDOWNLATCH_H

#include "libmevent/base/Condition.h"
#include "libmevent/base/Mutex.h"

namespace libmevent
{

class CountDownLatch : noncopyable
{
public:
    explicit CountDownLatch(int count);

    void wait();

    void countDown();

    int getCount() const;

private:
    mutable MutexLock mutex_;
    Condition condition_ GUARDED_BY(mutex_);
    int count_ GUARDED_BY(mutex_);
};

}  // namespace libmevent
#endif  // libmevent_BASE_COUNTDOWNLATCH_H
