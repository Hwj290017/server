#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include "timer.h"
#include <cassert>
#include <map>
#include <string>
class HttpRequest
{
  public:
    enum Method
    {
        Invalid,
        Get,
        Post,
        Head,
        Put,
        DeLete,
    };
    enum Version
    {
        Unknow,
        Http10,
        Http11,
    };

    HttpRequest() : method_(Invalid), version_(Unknow)
    {
    }

    Method method() const
    {
        return method_;
    }

    void setMethod(Method method)
    {
        method_ = method;
    }

    bool setMethod(const char* start, const char* end)
    {
        assert(method_ == Invalid);
        std::string m(start, end);
        if (m == "GET")
        {
            method_ = Get;
        }
        else if (m == "POST")
        {
            method_ = Post;
        }
        else if (m == "HEAD")
        {
            method_ = Head;
        }
        else if (m == "PUT")
        {
            method_ = Put;
        }
        else if (m == "DELETE")
        {
            method_ = DeLete;
        }
        else
        {
            method_ = Invalid;
        }
        return method_ != Invalid;
    }

    const char* methodString() const
    {
        const char* result = "UNKNOW";
        switch (method_)
        {
        case Get:
            result = "GET";
            break;
        case Post:
            result = "POST";
            break;
        case Head:
            result = "HEAD";
            break;
        case Put:
            result = "PUT";
            break;
        case DeLete:
            result = "DELETE";
            break;
        default:
            break;
        }
        return result;
    }

    Version version() const
    {
        return version_;
    }

    void setVersion(Version version)
    {
        version_ = version;
    }

    const std::string& path() const
    {
        return path_;
    }

    void setPath(const char* start, const char* end)
    {
        path_.assign(start, end);
    }

    const std::string& query() const
    {
        return query_;
    }

    void setQuery(const char* start, const char* end)
    {
        query_.assign(start, end);
    }

    const TimeSpec& receiveTime() const
    {
        return receiveTime_;
    }

    void setReceiveTime(const TimeSpec& time)
    {
        receiveTime_ = time;
    }

    const std::map<std::string, std::string>& headers() const
    {
        return headers_;
    }

    std::string getHeader(const std::string& field) const;

    void addHeader(const char* start, const char* colon, const char* end);

    const std::string& body() const
    {
        return body_;
    }

    void setBody(const char* start, const char* end)
    {
        body_.append(start, end);
    }

    void reset();

  private:
    Method method_;
    Version version_;
    std::string path_;
    std::string query_;
    TimeSpec receiveTime_;
    std::map<std::string, std::string> headers_;
    std::string body_;
};
#endif
