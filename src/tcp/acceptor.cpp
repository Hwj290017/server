#include "acceptor.h"
#include "iocontext.h"
#include "iocontextpool.h"
#include "log.h"
#include "socket.h"
namespace tcp
{
Acceptor::Acceptor(IoContext* ioContext, const InetAddress& addr)
    : IoObject(Socket::createAcceptorSocket(addr), ioContext), addr_(addr)
{
}

Acceptor::~Acceptor()
{
}

void Acceptor::start()
{
    ioContext_->runInThread([this]() { ioContext_->enableRead(this); });
}

// 接受客户端连接
void Acceptor::onRead()
{
    InetAddress clientAddr;
    auto clientSocket = Socket::accept(fd_, &clientAddr);
    IoContextPool::instance()->addConnection(clientSocket, clientAddr, this);
    Logger::logger << ("Accept a connection: " + std::to_string(clientSocket));
    Logger::logger << clientAddr.toIpPort();
}

void Acceptor::stop(double delay)
{
    ioContext_->remove(this);
}

} // namespace tcp
