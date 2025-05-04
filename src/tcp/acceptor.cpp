#include "tcp/acceptor.h"
#include "baseobjectimpl.h"
#include "socket.h"
#include "utils/log.h"
#include <memory>
namespace tcp
{
Acceptor::Acceptor(Acceptor&&) noexcept = default;
Acceptor::Acceptor(IoContext* ioContext, std::size_t id, BaseTasks&& baseTasks, ReleaseTask&& releaseTask,
                   InetAddress&& listenAddr, AcceptTask&& acceptTask)
{

    impl_ =
        std::make_unique<Impl<Acceptor>>(socket::createAcceptorSocket(listenAddr), ioContext, id, std::move(baseTasks),
                                         std::move(releaseTask), std::move(listenAddr), std::move(acceptTask));

    // 设置读事件
    impl_->channel_.setReadTask([this]() {
        InetAddress clientAddr;
        auto clientfd = socket::accept(impl_->fd_, &clientAddr);
        impl_->acceptTask_(this, clientfd, clientAddr);
        Logger::logger << ("Accept a connection: " + std::to_string(clientfd));
        Logger::logger << clientAddr.toIpPort();
    });
}

Acceptor::~Acceptor() = default;

} // namespace tcp
