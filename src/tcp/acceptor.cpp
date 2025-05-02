#include "acceptor.h"
#include "iocontext.h"
#include "log.h"
#include "object.h"
#include "sharedobject.h"
#include "socket.h"
#include <cstddef>
namespace tcp
{
Acceptor::Acceptor(std::size_t id, IoContext* ioContext, std::size_t parent, const InetAddress& addr)
    : SharedObject(socket::createAcceptorSocket(addr), ioContext, id, parent), addr_(addr)
{
}

Acceptor::~Acceptor()
{
}

void Acceptor::start()
{
    poller_->update(fd_, this, Poller::);
}

// 接受客户端连接
void Acceptor::onRead()
{
    InetAddress clientAddr;
    auto clientSocket = socket::accept(fd_, &clientAddr);

    Logger::logger << ("Accept a connection: " + std::to_string(clientSocket));
    Logger::logger << clientAddr.toIpPort();
}

void Acceptor::stop()
{
}

} // namespace tcp
