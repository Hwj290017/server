#include "tcp/server.h"
#include "connection.h"
#include "iocontextpool.h"
#include "sharedobjectpool.h"
#include "tcp/acceptorid.h"
#include "tcp/connectorid.h"
#include <cassert>
namespace tcp
{
// struct Server::Impl{};
AcceptorId Server::newAcceptor(const InetAddress& listenAddr, std::size_t parent)
{
    // acceptor创建
    IoContext* ioContext = nullptr;
    auto& objectPool = SharedObjectPool::instance();
    if (parent <= 0)
    {
        ioContext = IoContextPool::instance().getIoContext();
        return AcceptorId(objectPool.getAcceptor(listenAddr, ioContext), parent);
    }
    ioContext = objectPool.getIoContext(parent);
    return AcceptorId(objectPool.getAcceptor(listenAddr, ioContext), parent);
}

ConnectorId Server::newConnector(const InetAddress& serverAddr, std::size_t parent)
{
    IoContext* ioContext = nullptr;
    auto& objectPool = SharedObjectPool::instance();
    if (parent <= 0)
    {
        ioContext = IoContextPool::instance().getIoContext();
        return ConnectorId(objectPool.getAcceptor(serverAddr, ioContext), parent);
    }
    ioContext = objectPool.getIoContext(parent);
    return ConnectorId(objectPool.getAcceptor(serverAddr, ioContext), parent);
}

// 新连接创建
} // namespace tcp