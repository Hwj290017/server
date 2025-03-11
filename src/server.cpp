#include "server.h"
#include "acceptor.h"
#include "channel.h"
#include "connection.h"
#include "eventLoop.h"
#include "threadPool.h"
#include <cerrno>
#include <cfloat>
#include <functional>
#include <iostream>
#include <memory>
#include <unistd.h>

Server::Server(const char* ip, int port) : subReactors(THREADNUM), onConnectionCb([](Connection* conn) {})
{
    mainReactor = std::make_unique<EventLoop>();
    acceptor = std::make_unique<Acceptor>(mainReactor.get(), ip, port);
    thpool = std::make_unique<ThreadPool>(THREADNUM);
    // 启动subReactor线程
    for (int i = 0; i < subReactors.size(); ++i)
    {
        subReactors[i] = std::make_unique<EventLoop>();
        std::function<void()> cb = std::bind(&EventLoop::loop, subReactors[i].get());
        thpool->add(cb);
    }
    // 设置回调函数
    std::function<void(int)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCb(cb);
}

Server::~Server()
{
}

// 对客户端套接字建立新连接
void Server::newConnection(int clientFd)
{
    std::cout << "new connection: " << clientFd << std::endl;
    int random = clientFd % subReactors.size();
    std::unique_ptr<Connection> conn = std::make_unique<Connection>(clientFd, subReactors[random].get());
    // 设置回调函数
    conn->setDeleteConnectionCb(std::bind(&Server::deleteConnection, this, std::placeholders::_1));
    conn->setOnConnectionCb(onConnectionCb);
    connections[clientFd] = std::move(conn);
}
// 删除连接
void Server::deleteConnection(int clientFd)
{
    connections.erase(clientFd);
}
// 服务器注册事件
void Server::onConnection(std::function<void(Connection*)> cb)
{
    onConnectionCb = std::move(cb);
}

void Server::start() const
{
    // 启动主事件循环
    mainReactor->loop();
}