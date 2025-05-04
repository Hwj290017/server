#include "baseobjectpool.h"
#include "tcp/acceptor.h"
#include <cassert>

namespace tcp
{
size_t BaseObjectPool::nextId_ = 1;
BaseObjectPool::BaseObjectPool() = default;
BaseObjectPool::~BaseObjectPool() = default;
std::size_t BaseObjectPool::getAcceptor(const InetAddress& listenAddr, const Acceptor::BaseTasks& baseTasks,
                                        const Acceptor::AcceptTask& acceptTask, IoContext* ioContext)
{
    auto id = nextId_;
    // releaseTask由调用者负责线程安全
    auto releaseTask = [this](size_t id) {
        auto it = acceptors_.find(id);
        assert(it != acceptors_.end());
        acceptors_.erase(it);
    };
    ioContext->runTask([this, ioContext, id = nextId_, baseTasks, releaseTask = std::move(releaseTask), listenAddr,
                        acceptTask]() mutable {
        acceptors_.emplace(id, ioContext, id, std::move(baseTasks), std::move(releaseTask), std::move(listenAddr),
                           std::move(acceptTask));
    });
    nextId_++;
    return id;
}
std::size_t BaseObjectPool::getConnection(int clientfd, const InetAddress& peerAddr,
                                          const Connection::BaseTasks& baseTasks,
                                          const Connection::MessageTask& messageTask, IoContext* ioContext)
{
    auto id = nextId_;
    // releaseTask由调用者负责线程安全
    auto releaseTask = [this](size_t id) {
        auto it = connections_.find(id);
        assert(it != connections_.end());
        connections_.erase(it);
    };
    ioContext->runTask([this, clientfd, ioContext, id = nextId_, baseTasks, releaseTask = std::move(releaseTask),
                        peerAddr, messageTask] {
        connections_.emplace(id, clientfd, ioContext, id, std::move(baseTasks), std::move(releaseTask),
                             std::move(peerAddr), std::move(messageTask));
    });
    nextId_++;
    return id;
}
// 加入一个Connector
std::size_t BaseObjectPool::getConnector(const InetAddress& serverAddr, const Connector::BaseTasks& baseTasks,
                                         const Connector::MessageTask& messageTask, IoContext* ioContext)
{
    auto id = nextId_;
    // releaseTask由调用者负责线程安全
    auto releaseTask = [this](size_t id) {
        auto it = connections_.find(id);
        assert(it != connections_.end());
        connections_.erase(it);
    };
    ioContext->runTask(
        [this, ioContext, id = nextId_, baseTasks, releaseTask = std::move(releaseTask), serverAddr, messageTask] {
            connections_.emplace(id, ioContext, id, std::move(baseTasks), std::move(releaseTask), std::move(serverAddr),
                                 std::move(messageTask));
        });
    nextId_++;
    return id;
}
} // namespace tcp