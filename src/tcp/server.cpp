#include "tcp/server.h"
#include "baseobjectpool.h"
#include "iocontext.h"
#include "iocontextpool.h"
#include <cassert>
#include <cstddef>
#include <unordered_map>
namespace tcp
{
using size_t = std::size_t;
struct Server::Impl
{
    IoContextPool ioContextPool_;
    BaseObjectPool baseObjectPool_;
};
void Server::start()
{
    impl_->ioContextPool_.start();
}
// 加入一个Acceptor
std::size_t Server::newAcceptor(const InetAddress& listenAddr, const Acceptor::BaseTasks& baseTasks,
                                const Acceptor::AcceptTask& acceptTask)
{
    auto ioContext = impl_->ioContextPool_.getCurrentIoContext();
    return impl_->baseObjectPool_.getAcceptor(listenAddr, baseTasks, acceptTask, ioContext);
}
std::size_t Server::newConnection(int clientfd, const InetAddress& peerAddr, const Connection::BaseTasks& baseTasks,
                                  const Connection::MessageTask& messageTask)
{
    auto ioContext = impl_->ioContextPool_.getCurrentIoContext();
    return impl_->baseObjectPool_.getConnection(clientfd, peerAddr, baseTasks, messageTask, ioContext);
}
// 加入一个Connector
std::size_t Server::newConnector(const InetAddress& serverAddr, const Connector::BaseTasks& baseTasks,
                                 const Connector::MessageTask& messageTask)
{
    auto ioContext = impl_->ioContextPool_.getCurrentIoContext();
    return impl_->baseObjectPool_.getConnector(serverAddr, baseTasks, messageTask, ioContext);
}
// 线程安全，对id进行操作
template <typename T> void Server::doTask(std::size_t id, const std::function<void(TempPtr<T>)>& task)
{
    auto ioContext = impl_->ioContextPool_.getAttachedIoContext(id);
    impl_->baseObjectPool_.doTask(id, task, ioContext);
}
} // namespace tcp