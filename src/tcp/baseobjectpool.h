#pragma once

#include "iocontext.h"
#include "tcp/acceptor.h"
#include "tcp/connection.h"
#include "tcp/connector.h"
#include "tcp/inetaddress.h"
#include <cstddef>
#include <unordered_map>
#include <vector>
namespace tcp
{
using size_t = std::size_t;
// 目前没实现连接池的线程安全
class BaseObjectPool
{
  public:
    BaseObjectPool();
    ~BaseObjectPool();

    // 不考虑ioContext的线程安全问题
    std::size_t getAcceptor(const InetAddress& listenAddr, const Acceptor::BaseTasks& baseTasks,
                            const Acceptor::AcceptTask& acceptTask, IoContext* ioContext);
    std::size_t getConnection(int clientfd, const InetAddress& peerAddr, const Connection::BaseTasks& baseTasks,
                              const Connection::MessageTask& messageTask, IoContext* ioContext);
    // 加入一个Connector
    std::size_t getConnector(const InetAddress& serverAddr, const Connector::BaseTasks& baseTasks,
                             const Connector::MessageTask& messageTask, IoContext* ioContext);
    template <typename T>
    void doTask(std::size_t id, const std::function<void(TempPtr<T>)>& task, IoContext* ioContext);

  private:
    std::unordered_map<size_t, Acceptor> acceptors_;
    std::unordered_map<size_t, Connection> connections_;
    std::unordered_map<size_t, Connector> connectors_;
    // 从1开始，0保留用来表示不存在
    static std::size_t nextId_;
};
} // namespace tcp