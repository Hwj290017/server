#pragma once

#include "InetAddress.h"
#include "Socket.h"
#include "service.pb.h"
#include <string>

#define BUFFERLEN 1024
class RpcClient
{
    using String = std::string;

  public:
    RpcClient();
    ~RpcClient() = default;
    void connect(const InetAddress& serverAddr);
    // 目前输入输出为定义好的input和output
    template <typename Output, typename Input> auto call(const String& method, Input&& input) -> Output;

  private:
    Socket socket_;
    InetAddress serverAddr_;
};

template <typename Output, typename Input> auto RpcClient::call(const String& method, Input&& input) -> Output
{
    ::rpc::Request request;
    request.set_method(method);
    request.set_input(input.SerializeAsString());
    socket_.write(request.SerializeAsString());

    char buffer[BUFFERLEN];
    socket_.read(buffer, BUFFERLEN);
    ::rpc::Response response;
    response.ParseFromArray(buffer, BUFFERLEN);
    Output output;
    if (response.method() == method)
    {
        output.ParseFromString(response.output());
    }
    return output;
}
