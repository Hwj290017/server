#include "server.h"
#include "LoopThreadPool.h"
#include "acceptor.h"
#include "channel.h"
#include "connection.h"
#include "eventLoop.h"
#include <cerrno>
#include <cfloat>
#include <functional>
#include <iostream>
#include <memory>
#include <unistd.h>

Server::Server(const char* ip, int port) : mainLoop_(), threadPool_(&mainLoop_), onConnectionCb([](Connection* conn) {})
{
    acceptor_ = std::make_unique<Acceptor>(&mainLoop_, ip, port);
    // 设置回调函数
    std::function<void(int)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor_->setNewConnectionCb(cb);
    std::cout << "server created" << std::endl;
}

Server::~Server()
{
}

// 对客户端套接字建立新连接
void Server::newConnection(int clientFd)
{
    std::cout << "new connection: " << clientFd << std::endl;
    EventLoop* loop = threadPool_.nextLoop();
    std::unique_ptr<Connection> conn = std::make_unique<Connection>(clientFd, loop);
    // 设置回调函数
    conn->setDeleteConnectionCb(std::bind(&Server::deleteConnection, this, std::placeholders::_1));
    conn->setOnConnectionCb(onConnectionCb);

    connections_[clientFd] = std::move(conn);
}
// 删除连接
void Server::deleteConnection(int clientFd)
{
    connections_.erase(clientFd);
}
// 服务器注册事件
void Server::onConnection(std::function<void(Connection*)> cb)
{
    onConnectionCb = std::move(cb);
}

void Server::start()
{
    // 启动线程池
    threadPool_.start();
    // 启动主事件循环
    mainLoop_.loop();
}