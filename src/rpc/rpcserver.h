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
    void registerService(const std::string& method, std::function<String(String)> func);
    void stop();

  private:
    void onConnection(TcpConnection* conn);
    void onMessage(TcpConnection* conn, Buffer* buffer, const TimeSpec& recvTime);
    TcpServer server_;
    ServiceHandler handler;
};
