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
    void newAcceptor(const InetAddress& listenAddr, const Acceptor::BaseTasks& baseTasks,
                     const Acceptor::AcceptTask& acceptTask, const std::size_t parent = 0);
    void newConnection(int clientfd, const InetAddress& peerAddr, const Connection::BaseTasks& baseTasks,
                       const Connection::MessageTask& messageTask, const std::size_t parent = 0);
    // 加入一个Connector
    void newConnector(const InetAddress& serverAddr, const Connector::BaseTasks& baseTasks,
                      const Connector::MessageTask& messageTask, const std::size_t parent = 0);

    // 线程安全，对id进行操作
    template <typename T> void doTask(std::size_t id, const std::function<void(TempPtr<T>)>& task);
    std::size_t getParent(std::size_t id) const;
};

} // namespace tcp