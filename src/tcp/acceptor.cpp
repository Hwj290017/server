#include "tcp/acceptor.h"
#include "accptorimpl.h"
#include "socket.h"
#include "utils/log.h"
#include <memory>
namespace tcp
{
Acceptor::Acceptor(Acceptor&& other) noexcept = default;

Acceptor::Acceptor(IoContext* ioContext, std::size_t id, const InetAddress& listenAddr, const Tasks& tasks,
                   const ReleaseTask& releaseTask)
{

    impl_ = std::make_unique<Impl<Acceptor>>(socket::createAcceptorSocket(listenAddr), ioContext, id, listenAddr, tasks,
                                             releaseTask);

    // 设置读事件
    impl_->channel_.setReadTask([this]() {
        Logger::logger << std::string("Acceptor::readTask");
        InetAddress clientAddr;
        auto clientfd = socket::accept(impl_->fd_, &clientAddr);
        Logger::logger << ("Accept a connection: " + std::to_string(clientfd));
        Logger::logger << clientAddr.toIpPort();
        impl_->acceptTask_(this, clientfd, clientAddr);
    });
}
Acceptor::~Acceptor() = default;

} // namespace tcp
