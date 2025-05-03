#include "tcp/acceptor.h"
#include "baseobjectimpl.h"
#include "socket.h"
#include "util/log.h"
namespace tcp
{
struct Acceptor::Impl
{
    Impl(const InetAddress& listenAddr, const AcceptTask& acceptTask) : listenAddr_(listenAddr), acceptTask_(acceptTask)
    {
    }
    InetAddress listenAddr_;
    AcceptTask acceptTask_;
};
Acceptor::Acceptor(IoContext* ioContext, std::size_t id, const BaseTasks& baseTasks, const ReleaseTask& releaseTask,
                   const InetAddress& listenAddr, const AcceptTask& acceptTask)
    : BaseObject(socket::createAcceptorSocket(listenAddr), ioContext, id, baseTasks, releaseTask),
      impl_(std::make_unique<Impl>(listenAddr, acceptTask))
{

    // 设置读事件
    baseImpl_->channel_.setReadTask([this]() {
        InetAddress clientAddr;
        auto clientfd = socket::accept(baseImpl_->fd_, &clientAddr);
        impl_->acceptTask_(this, clientfd, clientAddr);
        Logger::logger << ("Accept a connection: " + std::to_string(clientfd));
        Logger::logger << clientAddr.toIpPort();
    });
}

Acceptor::~Acceptor() = default;

} // namespace tcp
