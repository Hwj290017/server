#include "acceptor.h"
namespace tcp
{
Acceptor::Acceptor(IoContext* ioContext, std::size_t id, const InetAddress& addr)
    : SharedObject(socket::createAcceptorSocket(addr), ioContext, id), addr_(addr)
{
}

Acceptor::~Acceptor()
{
}

// 接受客户端连接
void Acceptor::onRead()
{
    InetAddress clientAddr;
    auto clientSocket = socket::accept(fd_, &clientAddr);
    SharedObjectPool::instance().getConnection(clientSocket, clientAddr, id_);
    Logger::logger << ("Accept a connection: " + std::to_string(clientSocket));
    Logger::logger << clientAddr.toIpPort();
}

} // namespace tcp
