#include "httpContext.h"
#include "buffer.h"
#include "timer.h"

// 解析头行
bool HttpContext::processRequestLine(const char* begin, const char* end)
{
    bool succeed = false;
    const char* start = begin;
    const char* space = std::find(start, end, ' ');
    if (space != end && request_.setMethod(start, space))
    {
        start = space + 1;
        space = std::find(start, end, ' ');
        if (space != end)
        {
            const char* question = std::find(start, space, '?');
            if (question != space)
            {
                request_.setPath(start, question);
                request_.setQuery(question, space);
            }
            else
            {
                request_.setPath(start, space);
            }
            start = space + 1;
            succeed = end - start == 8 && std::equal(start, end - 1, "HTTP/1.");
            if (succeed)
            {
                if (*(end - 1) == '1')
                {
                    request_.setVersion(HttpRequest::Http11);
                }
                else if (*(end - 1) == '0')
                {
                    request_.setVersion(HttpRequest::Http10);
                }
                else
                {
                    succeed = false;
                }
            }
        }
    }
    return succeed;
}

bool HttpContext::parseRequest(const Buffer& buf, const TimeSpec& receiveTime)
{
    bool ok = true;
    bool hasMore = true;
    const char* begin = buf.begin();
    const char* end = buf.end();
    const char* now = begin;
    while (hasMore)
    {
        if (state_ == ExpectRequestLine)
        {
            const char* crlf = buf.findCRLF(now);
            if (crlf != end)
            {
                ok = processRequestLine(begin, crlf);
                if (ok)
                {
                    request_.setReceiveTime(receiveTime);
                    state_ = ExpectHeaders;
                    now = crlf + 2;
                }
                else
                {
                    hasMore = false;
                }
            }
            else
            {
                hasMore = false;
            }
        }
        else if (state_ == ExpectHeaders)
        {
            const char* crlf = buf.findCRLF(now);
            if (crlf != end)
            {

                if (crlf != now)
                {
                    const char* colon = std::find(now, crlf, ':');
                    if (colon != crlf)
                    {
                        request_.addHeader(now, colon, crlf);
                        now = crlf + 2;
                    }
                }
                // 空行
                else
                {
                    state_ = ExpectBody;
                    now = crlf + 2;
                }
            }
            else
            {
                hasMore = false;
            }
        }
        else if (state_ == ExpectBody)
        {
            request_.setBody(now, end);
            state_ = GotAll;
            hasMore = false;
        }
    }
    return ok;
}
