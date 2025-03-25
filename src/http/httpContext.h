

#ifndef HTTPCONTEXT_H
#define HTTPCONTEXT_H

#include "buffer.h"
#include "httpRequest.h"
#include "timer.h"

class HttpContext
{
  public:
    enum HttpRequestParseState
    {
        ExpectRequestLine,
        ExpectHeaders,
        ExpectBody,
        GotAll,
    };

    HttpContext() : state_(ExpectRequestLine)
    {
    }

    // default copy-ctor, dtor and assignment are fine

    // return false if any error
    bool parseRequest(const Buffer& buf, const TimeSpec& receiveTime);

    bool gotAll() const
    {
        return state_ == GotAll;
    }

    void reset()
    {
        state_ = ExpectRequestLine;
        request_.reset();
    }

    const HttpRequest& request() const
    {
        return request_;
    }

    HttpRequest& request()
    {
        return request_;
    }

  private:
    bool processRequestLine(const char* begin, const char* end);

    HttpRequestParseState state_;
    HttpRequest request_;
};

#endif // HTTPCONTEXT_H