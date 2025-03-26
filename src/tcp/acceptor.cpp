#include "acceptor.h"
#include "eventLoop.h"
#include "log.h"
#include <string>

Acceptor::Acceptor(EventLoop* loop, const InetAddress& addr)
    : loop_(loop), socket_(Socket::createNonBlocking(addr)), channel_(&socket_), addr_(addr)
{

    socket_.bind(addr);
    socket_.listen();

    channel_.setReadCb(std::bind(&Acceptor::handleRead, this));
    channel_.enableRead();
    channel_.start();
    loop_->updateChannel(&channel_);
}

Acceptor::~Acceptor()
{
    channel_.close();
    loop_->updateChannel(&channel_);
}

// 接受客户端连接
void Acceptor::handleRead() const
{
    InetAddress clientAddr;

    Socket clientSocket = socket_.accept(clientAddr);
    Logger::logger << ("Accept a connection: " + std::to_string(clientSocket.fd()));
    // 交给server建立连接
    if (newConnectionCb_)
        newConnectionCb_(std::move(clientSocket), clientAddr);
}
