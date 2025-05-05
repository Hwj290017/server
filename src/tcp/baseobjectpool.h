#pragma once

#include "iocontext.h"
#include "tcp/acceptor.h"
#include "tcp/connection.h"
#include "tcp/connector.h"
#include "tcp/inetaddress.h"
#include <cstddef>
#include <memory>
#include <mutex>
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
    std::size_t getAcceptor(const InetAddress& listenAddr, const Acceptor::Tasks& tasks, IoContext* ioContext);
    std::size_t getConnection(int clientfd, const InetAddress& peerAddr, const Connection::Tasks& tasks,
                              IoContext* ioContext);
    // 加入一个Connector
    std::size_t getConnector(const InetAddress& serverAddr, const Connector::Tasks& tasks, IoContext* ioContext);
    template <typename T>
    void doTask(std::size_t id, const std::function<void(TempPtr<T>)>& task, IoContext* ioContext);

  private:
    std::unordered_map<size_t, std::unique_ptr<Acceptor>> acceptors_;
    std::unordered_map<size_t, std::unique_ptr<Connection>> connections_;
    std::unordered_map<size_t, std::unique_ptr<Connector>> connectors_;
    // 对三个map的访问需要加锁，后续优化
    std::mutex mutex_;
    // 从1开始，0保留用来表示不存在
    static std::size_t nextId_;
};
} // namespace tcp