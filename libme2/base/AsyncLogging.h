// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef libmevent_BASE_ASYNCLOGGING_H
#define libmevent_BASE_ASYNCLOGGING_H

// #include "libmevent/base/BlockingQueue.h"
// #include "libmevent/base/BoundedBlockingQueue.h"
#include "libmevent/base/CountDownLatch.h"
#include "libmevent/base/Mutex.h"
#include "libmevent/base/Thread.h"
#include "libmevent/base/LogStream.h"

#include <atomic>
#include <vector>

namespace libmevent
{

class AsyncLogging : noncopyable
{
public:
    AsyncLogging(const string& basename, off_t rollSize, int flushInterval = 3);

    ~AsyncLogging()
    {
        if(running_) {
            stop();
        }
    }

    void append(const char* logline, int len);

    void start()
    {
        running_ = true;
        thread_.start();
        latch_.wait();
    }

    void stop() NO_THREAD_SAFETY_ANALYSIS
    {
        running_ = false;
        cond_.notify();
        thread_.join();
    }

private:
    void threadFunc();

    typedef libmevent::detail::FixedBuffer<libmevent::detail::kLargeBuffer> Buffer;
    typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
    typedef BufferVector::value_type BufferPtr;

    const int flushInterval_;
    std::atomic<bool> running_;
    const string basename_;
    const off_t rollSize_;
    libmevent::Thread thread_;
    libmevent::CountDownLatch latch_;
    libmevent::MutexLock mutex_;
    libmevent::Condition cond_ GUARDED_BY(mutex_);
    BufferPtr currentBuffer_ GUARDED_BY(mutex_);
    BufferPtr nextBuffer_ GUARDED_BY(mutex_);
    BufferVector buffers_ GUARDED_BY(mutex_);
};

}  // namespace libmevent

#endif  // libmevent_BASE_ASYNCLOGGING_H
