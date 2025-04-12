#pragma once

#include "TcpServer.h"
#include "timer.h"
#include <map>
#include <memory>
class InetAddress;
class Service;
class TcpConnection;
class RpcServer
{
    using ServicePtr = std::unique_ptr<Service>;

  public:
    RpcServer(const InetAddress& addr);
    void start();
    void registerService(const Service& service);
    void stop();

  private:
    void onConnection(TcpConnection* conn);
    void onMessage(TcpConnection* conn, Buffer* buf, const TimeSpec& recvTime);
    TcpServer server_;
    std::map<std::string, ServicePtr> services_;
};