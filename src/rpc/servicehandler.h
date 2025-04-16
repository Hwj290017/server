#pragma once

#include "service.pb.h"
#include <functional>
#include <map>
class ServiceHandler
{
    using Response = ::rpc::Response;
    using Request = ::rpc::Request;
    using String = std::string;
    using Services = std::map<String, std::function<String(String)>>;

  public:
    // 调用服务
    Response call(const Request& request);
    // 注册服务，全部转成function
    void registerService(const String& method, std::function<String(String)> func);

  private:
    Services services_;
};
