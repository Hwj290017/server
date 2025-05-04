#pragma once
#include "tcp/acceptor.h"
#include "tcp/connector.h"
#include "tcp/tempptr.h"
#include <cstddef>
#include <functional>

namespace tcp
{
class InetAddress;
class IoContext;
class Server
{
  public:
    void start();
    // 加入一个Acceptor
    std::size_t newAcceptor(const InetAddress& listenAddr, const Acceptor::Tasks& tasks);
    std::size_t newConnection(int clientfd, const InetAddress& peerAddr, const Connection::Tasks& tasks);
    // 加入一个Connector
    std::size_t newConnector(const InetAddress& serverAddr, const Connector::Tasks& tasks);

    // 线程安全，对id进行操作
    template <typename T> void doTask(std::size_t id, const std::function<void(TempPtr<T>)>& task);

  private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace tcp