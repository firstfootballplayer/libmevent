// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef libmevent_BASE_LOGFILE_H
#define libmevent_BASE_LOGFILE_H

#include "libmevent/base/Mutex.h"
#include "libmevent/base/Types.h"

#include <memory>

namespace libmevent
{

namespace FileUtil
{
    class AppendFile;
}

class LogFile : noncopyable
{
public:
    LogFile(const string& basename, off_t rollSize, bool threadSafe = true, int flushInterval = 3, int checkEveryN = 1024);
    ~LogFile();

    void append(const char* logline, int len);
    void flush();
    bool rollFile();

private:
    void append_unlocked(const char* logline, int len);

    static string getLogFileName(const string& basename, time_t* now);

    const string basename_;
    const off_t rollSize_;
    const int flushInterval_;
    const int checkEveryN_;

    int count_;

    std::unique_ptr<MutexLock> mutex_;
    time_t startOfPeriod_;
    time_t lastRoll_;
    time_t lastFlush_;
    std::unique_ptr<FileUtil::AppendFile> file_;

    const static int kRollPerSeconds_ = 60 * 60 * 24;
};

}  // namespace libmevent
#endif  // libmevent_BASE_LOGFILE_H
