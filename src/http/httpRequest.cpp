#include "httpRequest.h"

void HttpRequest::addHeader(const char* start, const char* colon, const char* end)
{
    std::string field(start, colon);
    ++colon;
    while (colon < end && isspace(*colon))
    {
        ++colon;
    }
    std::string value(colon, end);
    while (!value.empty() && isspace(value[value.size() - 1]))
    {
        value.resize(value.size() - 1);
    }
    headers_[field] = value;
}

std::string HttpRequest::getHeader(const std::string& field) const
{
    std::string result;
    std::map<std::string, std::string>::const_iterator it = headers_.find(field);
    if (it != headers_.end())
    {
        result = it->second;
    }
    return result;
}

void HttpRequest::reset()
{
    method_ = Invalid;
    version_ = Unknow;
    path_.clear();
    query_.clear();
    receiveTime_ = TimeSpec::inValidExpired;
    headers_.clear();
    body_.clear();
}