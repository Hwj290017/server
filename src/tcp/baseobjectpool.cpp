#include "baseobjectpool.h"
#include "iocontext.h"
#include "tcp/acceptor.h"
#include "tcp/connection.h"
#include "tcp/connector.h"
#include "tcp/tempptr.h"
#include <cassert>
#include <memory>

namespace tcp
{
size_t BaseObjectPool::nextId_ = 1;
BaseObjectPool::BaseObjectPool() = default;
BaseObjectPool::~BaseObjectPool() = default;
std::size_t BaseObjectPool::getAcceptor(const InetAddress& listenAddr, const Acceptor::Tasks& tasks,
                                        IoContext* ioContext)
{
    auto id = nextId_;
    // releaseTask由调用者负责线程安全
    auto releaseTask = [this](size_t id) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = acceptors_.find(id);
        assert(it != acceptors_.end());
        acceptors_.erase(it);
    };
    {
        std::lock_guard<std::mutex> lock(mutex_);
        acceptors_.emplace(id, std::make_unique<Acceptor>(ioContext, id, listenAddr, tasks, releaseTask));
    }
    // 用下标符会报错
    auto it = acceptors_.find(id);
    ioContext->runTask([it]() { it->second->start(); });
    nextId_++;
    return id;
}
std::size_t BaseObjectPool::getConnection(int clientfd, const InetAddress& peerAddr, const Connection::Tasks& tasks,
                                          IoContext* ioContext)
{
    auto id = nextId_;
    // releaseTask由调用者负责线程安全
    auto releaseTask = [this](size_t id) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = connections_.find(id);
        assert(it != connections_.end());
        connections_.erase(it);
    };
    {
        std::lock_guard<std::mutex> lock(mutex_);
        connections_.emplace(id, std::make_unique<Connection>(clientfd, ioContext, id, peerAddr, tasks, releaseTask));
        // connections_[id]
    }
    auto it = connections_.find(id);
    ioContext->runTask([it]() { it->second->start(); });
    nextId_++;
    return id;
}
// 加入一个Connector
std::size_t BaseObjectPool::getConnector(const InetAddress& serverAddr, const Connector::Tasks& tasks,
                                         IoContext* ioContext)
{
    auto id = nextId_;
    // releaseTask由调用者负责线程安全
    auto releaseTask = [this](size_t id) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = connectors_.find(id);
        assert(it != connectors_.end());
        connectors_.erase(it);
    };
    {
        std::lock_guard<std::mutex> lock(mutex_);
        connectors_.emplace(id, std::make_unique<Connector>(ioContext, id, serverAddr, tasks, releaseTask));
    }
    auto it = connectors_.find(id);
    ioContext->runTask([it]() { it->second->start(); });
    nextId_++;
    return id;
}

IoContext* BaseObjectPool::getAttachedIoContext(size_t id) const
{
    auto it = attachedIoContexts_.find(id);
    if (it == attachedIoContexts_.end())
        return nullptr;
    return it->second;
}
// template<typename T> void BaseObjectPool::
// void BaseObjectPool::doTask<>()
} // namespace tcp