#include "httpResponse.h"
#include "buffer.h"
void HttpResponse::message(Buffer& message) const
{
    message.clear();
    message += "HTTP/1.1 ";
    message += std::to_string(statusCode_);
    message += " ";
    message += statusMessage_;
    message += "\r\n";
    if (closeConnection_)
    {
        message += "Connection: close\r\n";
    }
    else
    {
        message += "Content-Length: ";
        message += std::to_string(body_.size());
        message += "\r\n";
        message += "Connection: Keep-Alive\r\n";
    }

    for (const auto& header : headers_)
    {
        message += header.first;
        message += ": ";
        message += header.second;
        message += "\r\n";
    }

    message += "\r\n";
    message += body_;
}