#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <map>
#include <string>

class Buffer;
class HttpResponse
{
    using String = std::string;
    using Headers = std::map<std::string, std::string>;

  public:
    enum HttpStatusCode
    {
        Unknow,
        Ok = 200,
        MovedPermanently = 301,
        BadRequest = 400,
        NotFound = 404,
    };

    explicit HttpResponse(bool close) : statusCode_(Unknow), closeConnection_(close)
    {
    }

    void message(Buffer& message) const;

    void setStatusCode(HttpStatusCode code)
    {
        statusCode_ = code;
    }

    void setStatusMessage(const String& message)
    {
        statusMessage_ = message;
    }

    void setCloseConnection(bool on)
    {
        closeConnection_ = on;
    }

    bool closeConnection() const
    {
        return closeConnection_;
    }

    void setContentType(const String& contentType)
    {
        addHeader("Content-Type", contentType);
    }

    // FIXME: replace string with StringPiece
    void addHeader(const String& key, const String& value)
    {
        headers_[key] = value;
    }

    void setBody(const String& body)
    {
        body_ = body;
    }

  private:
    Headers headers_;
    HttpStatusCode statusCode_;
    // FIXME: add http version
    String statusMessage_;
    String body_;
    bool closeConnection_;
};

#endif