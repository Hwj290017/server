#pragma once

#include "service.pb.h"
#include <functional>
#include <google/protobuf/message.h>
#include <map>
#include <memory>
class ServiceHandler
{
    using Response = ::rpc::Response;
    using Request = ::rpc::Request;
    using String = std::string;
    using Services = std::map<String, std::function<String(String)>>;

    using Message = ::google::protobuf::Message;
    using MessagePtr = std::shared_ptr<Message>;

  public:
    // 调用服务
    Response call(const Request& request);
    // 注册服务，全部转成function
    template <typename R, typename Arg> void registerService(const String& method, std::function<R(Arg)> func);

    // 对不同参数函数进行包装
    template <typename R, typename Arg>
    static auto wrap(const std::function<R(Arg)>& func, const String& request) -> String;

  private:
    Services services_;
};

template <typename R, typename Arg>
void ServiceHandler::registerService(const String& method, std::function<R(Arg)> func)
{
    services_[method] = [func = std::move(func)](const String& request) -> String {
        return wrap<R, Arg>(func, request);
    };
}

template <typename R, typename Arg>
auto ServiceHandler::wrap(const std::function<R(Arg)>& func, const String& request) -> String
{
    // 判断子类
    static_assert(std::is_base_of<Message, Arg>::value, "Arg error");
    static_assert(std::is_base_of<Message, R>::value, "R error");
    Arg arg;
    static_cast<Message*>(&arg)->ParseFromString(request);
    auto res = func(arg);
    return static_cast<Message*>(&res)->SerializeAsString();
}
