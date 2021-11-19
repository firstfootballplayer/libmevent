// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/libmevent/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//

#include "libmevent/net/HttpContext.h"
#include "libmevent/net/Buffer.h"
// #include <string.h>
using namespace libmevent;
using namespace libmevent::net;

bool HttpContext::processRequestLine(const char* begin, const char* end)
{
    bool succeed = false;
    const char* start = begin;
    const char* space = std::find(start, end, ' ');
    if(space != end && request_.setMethod(start, space)) {
        start = space + 1;
        space = std::find(start, end, ' ');
        if(space != end) {
            const char* question = std::find(start, space, '?');
            if(question != space) {
                request_.setPath(start, question);
                request_.setQuery(question, space);
            } else {
                request_.setPath(start, space);
            }
            start = space + 1;
            succeed = end - start == 8 && std::equal(start, end - 1, "HTTP/1.");
            if(succeed) {
                if(*(end - 1) == '1') {
                    request_.setVersion(HttpRequest::kHttp11);
                } else if(*(end - 1) == '0') {
                    request_.setVersion(HttpRequest::kHttp10);
                } else {
                    succeed = false;
                }
            }
        }
    }
    return succeed;
}

// return false if any error
bool HttpContext::parseRequest(Buffer* buf, Timestamp receiveTime)
{
    bool ok = true;
    bool hasMore = true;
    // printf("%s\n", buf->peek());
    while(hasMore) {
        if(state_ == kExpectRequestLine) {
            const char* crlf = buf->findCRLF();
            if(crlf) {
                ok = processRequestLine(buf->peek(), crlf);
                if(ok) {
                    request_.setReceiveTime(receiveTime);
                    buf->retrieveUntil(crlf + 2);
                    state_ = kExpectHeaders;
                } else {
                    hasMore = false;
                }
            } else {
                hasMore = false;
            }
        } else if(state_ == kExpectHeaders) {
            const char* crlf = buf->findCRLF();
            if(crlf) {
                const char* colon = std::find(buf->peek(), crlf, ':');
                if(colon != crlf) {
                    request_.addHeader(buf->peek(), colon, crlf);
                } else {
                    // empty line, end of header
                    // FIXME:
                    if(request_.method() == HttpRequest::Method::kPost) {
                        state_ = kExpectBody;
                    } else {
                        state_ = kGotAll;
                        hasMore = false;
                    }
                }
                buf->retrieveUntil(crlf + 2);
            } else {
                hasMore = false;
            }
        } else if(state_ == kExpectBody) {
            // FIXME:
            int Length = 0;
            const std::map<string, string>& headers = request_.headers();
            for(const auto& header : headers) {
                if(!strcasecmp(header.first.c_str(), "Content-Length")) {
                    Length = stoi(header.second);
                    break;
                }
            }
            // printf("%d\n", Length);
            request_.setBody(buf->peek(), buf->peek() + Length);
            // printf("%s\n", request_.body().c_str());
            buf->retrieveUntil(buf->peek() + Length);
            // printf("%d\n", buf->readableBytes());
            hasMore = false;
            state_ = kGotAll;
        }
    }
    return ok;
}
