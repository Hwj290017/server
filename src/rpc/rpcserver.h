#pragma once

#include "service.h"
#include "servicehandler.h"
#include "tcpserver.h"
#include "timer.h"
#include <functional>
#include <memory>
#include <string>

class InetAddress;
class TcpConnection;
class RpcServer
{
    using ServicePtr = std::unique_ptr<ServiceImpl>;
    using String = std::string;

  public:
    RpcServer(const InetAddress& addr);
    ~RpcServer();
    void start();
    template <typename R, typename Arg> void registerService(const std::string& method, std::function<R(Arg)> func);
    // 把这个逻辑写到handler
    template <typename R, typename Arg> void registerService(const String& method, R (*func)(Arg));
    void stop();

  private:
    void onConnection(TcpConnection* conn);
    void onMessage(TcpConnection* conn, Buffer* buffer, const TimeSpec& recvTime);
    TcpServer server_;
    ServiceHandler handler_;
};
template <typename R, typename Arg>
void RpcServer::registerService(const std::string& method, std::function<R(Arg)> func)
{
    handler_.registerService(method, std::move(func));
}

template <typename R, typename Arg> void RpcServer::registerService(const String& method, R (*func)(Arg))
{
    registerService(method, std::function<R(Arg)>(func));
}