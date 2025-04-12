#include "acceptor.h"
#include "eventLoop.h"
#include "log.h"
#include <iostream>
#include <string>

Acceptor::Acceptor(EventLoop* loop, const InetAddress& addr)
    : loop_(loop), socket_(Socket::createServerSocket(addr)), channel_(&socket_), addr_(addr)
{
    socket_.listen();
    channel_.setReadCb([this]() { handleRead(); });
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
    Logger::logger << clientAddr.toIpPort();
    // 交给server建立连接
    if (newConnectionCb_)
        newConnectionCb_(std::move(clientSocket), clientAddr);
}
