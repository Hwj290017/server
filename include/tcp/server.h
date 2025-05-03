#pragma once
#include "tcp/acceptor.h"
#include "tcp/connector.h"
#include "tcp/object.h"

namespace tcp
{
class InetAddress;
class Server
{
  public:
    void start();
    // 加入一个Acceptor
    void newAcceptor(const InetAddress& listenAddr, const Acceptor::Tasks& tasks, const std::size_t parent = 0);
    // 加入一个Connector
    void newConnector(const InetAddress& serverAddr, const Connector::Tasks& tasks, const std::size_t parent = 0);

    void
};

} // namespace tcp