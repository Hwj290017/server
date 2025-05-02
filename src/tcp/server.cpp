#include "server.h"
#include "connection.h"
#include "ioobject.h"
#include "sharedioobject.h"
#include <cassert>
#include <memory>
namespace tcp
{

auto Server::makeAcceptor(const InetAddress& listenAddr, SharedIoObject* parent) -> std::shared_ptr<Acceptor>
{
    // 最开始的acceptor创建
    IoContext* ioContext = nullptr;
    if (!parent)
    {
        ioContext = ioContextpool_.getIoContext();
        return std::make_shared<Acceptor>(ioContext, listenAddr, nullptr);
    }
    ioContext = parent->ioContext();
    return std::make_shared<Acceptor>(ioContext, listenAddr, parent->weak_from_this());
}

auto Server::makeConnection(int sockfd, const InetAddress& peerAddr, SharedIoObject* parent)
    -> std::shared_ptr<Connection>
{
    assert(sockfd >= 0 && parent != nullptr);
    auto ioContext = ioContextpool_.getIoContext();
    return std::make_shared<Connection>(ioContext, sockfd, peerAddr, parent->weak_from_this());
}
// 新连接创建
} // namespace tcp