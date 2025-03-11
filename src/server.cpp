#include "server.h"
#include "acceptor.h"
#include "channel.h"
#include "connection.h"
#include "eventLoop.h"
#include "socket.h"
#include "threadPool.h"
#include <cerrno>
#include <cfloat>
#include <functional>
#include <unistd.h>

Server::Server(EventLoop* loop)
    : mainReactor(loop), acceptor(new Acceptor(loop)), subReactors(THREADNUM), thpool(new ThreadPool(THREADNUM)),
      onConnectionCb([](Connection* conn) {})
{

    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCb(cb);

    // 启动subReactor线程
    for (int i = 0; i < subReactors.size(); ++i)
    {
        subReactors[i] = new EventLoop();
        std::function<void()> cb = std::bind(&EventLoop::loop, subReactors[i]);
        thpool->add(cb);
    }
}

Server::~Server()
{
    delete acceptor;
    delete thpool;
    for (int i = 0; i < subReactors.size(); ++i)
        delete subReactors[i];
}

// 对客户端套接字建立新连接
void Server::newConnection(Socket* clientSock)
{
    int random = clientSock->getFd() % subReactors.size();
    Connection* conn = new Connection(clientSock, subReactors[random]);
    // 设置回调函数
    conn->setDeleteConnectionCb(std::bind(&Server::deleteConnection, this, std::placeholders::_1));
    conn->setOnConnectionCb(onConnectionCb);

    connections[clientSock->getFd()] = conn;
}
// 删除连接
void Server::deleteConnection(Socket* clientSock)
{
    Connection* conn = connections[clientSock->getFd()];
    connections.erase(clientSock->getFd());
    delete conn;
}
// 服务器注册事件
void Server::onConnection(std::function<void(Connection*)> cb)
{
    onConnectionCb = std::move(cb);
}