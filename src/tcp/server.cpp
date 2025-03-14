#include "server.h"
#include "LoopThreadPool.h"
#include "acceptor.h"
#include "channel.h"
#include "connection.h"
#include "eventLoop.h"
#include "log.h"
#include <cerrno>
#include <cfloat>
#include <functional>
#include <iostream>
#include <memory>
#include <unistd.h>

Server::Server(const char* ip, int port) : mainLoop_(), threadPool_(&mainLoop_), readCb([](Connection* conn) {})
{
    acceptor_ = std::make_unique<Acceptor>(&mainLoop_, ip, port);
    // 设置回调函数
    std::function<void(int)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor_->setNewConnectionCb(cb);
    logger << "Server created\n";
}

Server::~Server()
{
}

// 对客户端套接字建立新连接
void Server::newConnection(int clientFd)
{
    EventLoop* loop = threadPool_.nextLoop();
    std::unique_ptr<Connection> conn = std::make_unique<Connection>(clientFd, loop);
    // 设置回调函数
    conn->setCloseCb(std::bind(&Server::closeConnection, this, std::placeholders::_1));
    conn->setReadCb(readCb);
    connections_[clientFd] = std::move(conn);
    logger << "new connection: " << clientFd << "\n";
}
// 删除连接
void Server::closeConnection(int clientFd)
{
    connections_.erase(clientFd);
    logger << "connection closed: " << clientFd << "\n";
}
// 服务器注册读事件
void Server::setReadCb(std::function<void(Connection*)> cb)
{
    readCb = std::move(cb);
}

void Server::start()
{
    logger << "Server started\n";
    // 启动线程池
    threadPool_.start();
    // 启动主事件循环
    mainLoop_.loop();
}