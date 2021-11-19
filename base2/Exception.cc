// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "libmevent/base/Exception.h"
#include "libmevent/base/CurrentThread.h"

namespace libmevent
{

Exception::Exception(string msg) : message_(std::move(msg)), stack_(CurrentThread::stackTrace(/*demangle=*/false))
{
}

}  // namespace libmevent
