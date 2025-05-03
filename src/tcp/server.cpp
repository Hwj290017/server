#include "tcp/server.h"
#include "manager.h"
#include <cassert>
namespace tcp
{
// struct Server::Impl{};
void Server::newAcceptor(const InetAddress& listenAddr, const Acceptor::Tasks& tasks, const Object* parent)
{
    Manager::instance().newAcceptor(listenAddr, tasks, parent);
}

void Server::newConnector(const InetAddress& serverAddr, const Connector::Tasks& tasks, const Object* parent)
{
    Manager::instance().newConnector(serverAddr, tasks, parent);
}

// 新连接创建
} // namespace tcp