#pragma once

#include "tcp/acceptor.h"
#include "tcp/connection.h"
#include "tcp/connector.h"
#include "tcp/inetaddress.h"
#include <cstddef>
#include <memory>
#include <mutex>
#include <unordered_map>
namespace tcp
{
using size_t = std::size_t;
class IoContext;
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
    IoContext* getAttachedIoContext(std::size_t id) const;

  private:
    // 对三个map的访问需要加锁，后续优化
    std::mutex mutex_;
    // 从1开始，0保留用来表示不存在
    static std::size_t nextId_;
};
} // namespace tcp