#include "TcpServer.h"
#include "LoopThreadPool.h"
#include "Socket.h"
#include "TcpConnection.h"
#include "acceptor.h"
#include "eventLoop.h"
#include <cerrno>
#include <cfloat>
#include <functional>
#include <iostream>
#include <memory>
#include <unistd.h>

TcpServer::TcpServer(const InetAddress& addr, const std::string& name)
    : localAddr_(addr), name_(name), mainLoop_(), threadPool_()
{
    std::cout << "TcpServer created\n";
    // 创建Acceptor
    acceptor_ = std::make_unique<Acceptor>(&mainLoop_, addr);
    // 设置回调函数
    NewConnectionCb cb = std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2);
    acceptor_->setNewConnectionCb(cb);
}

TcpServer::~TcpServer() = default;

void TcpServer::start()
{
    // 启动线程池
    threadPool_.start();
    // 启动主事件循环
    mainLoop_.loop();
}

// 对客户端套接字建立新连接
void TcpServer::newConnection(Socket&& clientSock, const InetAddress& peerAddr)
{
    EventLoop* loop = threadPool_.nextLoop();
    TcpConnectionPtr conn =
        std::make_unique<TcpConnection>(std::move(clientSock), loop, localAddr_, peerAddr, "c", nextConnId_);
    // 设置回调函数
    conn->setCloseCb(std::bind(&TcpServer::closeConnection, this, std::placeholders::_1));
    conn->setMessageCb(messageCb_);
    conn->setOnConnectionCb(onConnectionCb_);
    // 启动连接
    conn->start();
    connections_[nextConnId_] = std::move(conn);
    ++nextConnId_;
}
// 删除连接
void TcpServer::closeConnection(int connid)
{
    // 由主线程删除连接
    mainLoop_.runInLoop(std::bind(&TcpServer::closeConnectionInLoop, this, connid));
}

void TcpServer::closeConnectionInLoop(int connid)
{
    connections_.erase(connid);
    std::cout << "Connection " << connid << " closed\n";
}

int TcpServer::nextConnId_ = 0;
