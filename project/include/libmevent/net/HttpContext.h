// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/libmevent/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef libmevent_NET_HTTP_HTTPCONTEXT_H
#define libmevent_NET_HTTP_HTTPCONTEXT_H

#include "libmevent/base/copyable.h"

#include "libmevent/net/HttpRequest.h"

namespace libmevent {
namespace net {

class Buffer;

class HttpContext : public libmevent::copyable {
public:
  enum HttpRequestParseState {
    kExpectRequestLine,
    kExpectHeaders,
    kExpectBody,
    kGotAll,
  };

  HttpContext() : state_(kExpectRequestLine) {}

  // default copy-ctor, dtor and assignment are fine

  // return false if any error
  bool parseRequest(Buffer *buf, Timestamp receiveTime);

  bool gotAll() const { return state_ == kGotAll; }

  void reset() {
    state_ = kExpectRequestLine;
    HttpRequest dummy;
    request_.swap(dummy);
  }

  const HttpRequest &request() const { return request_; }

  HttpRequest &request() { return request_; }

private:
  bool processRequestLine(const char *begin, const char *end);

  HttpRequestParseState state_;
  HttpRequest request_;
};

} // namespace net
} // namespace libmevent

#endif // libmevent_NET_HTTP_HTTPCONTEXT_H
