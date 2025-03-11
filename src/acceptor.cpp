#include "acceptor.h"
#include "channel.h"
#include "socket.h"

Acceptor::Acceptor(EventLoop* loop) : loop(loop), sock(new Socket()), acceptChannel(new Channel(loop, sock))
{
    InetAddress* addr = new InetAddress(LOCALHOST, PORT);
    sock->bind(addr);
    sock->listen();
    sock->setnonblocking();
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    acceptChannel->setCallback(cb);
    acceptChannel->enableReading();
    delete addr;
}

Acceptor::~Acceptor()
{
    delete sock;
    delete acceptChannel;
}

// 接受客户端连接
void Acceptor::acceptConnection() const
{
    InetAddress* addr = new InetAddress();
    Socket* clientSock = new Socket(sock->accept(addr));
    // 交给server建立连接
    newConnectionCb(clientSock);
    delete addr;
}

void Acceptor::setNewConnectionCb(std::function<void(Socket*)> cb)
{
    newConnectionCb = cb;
}