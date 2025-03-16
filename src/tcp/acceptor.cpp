#include "acceptor.h"
#include "channel.h"
#include "eventLoop.h"
#include "util.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

Acceptor::Acceptor(EventLoop* loop, const char* ip, int port) : fd_(-1), loop_(loop)
{
    // 地址
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    // 创建socket
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd_ < 0, "Socket create error");
    errif(bind(fd_, (sockaddr*)&addr, sizeof(addr)) < 0, "Socket bind error");
    errif(::listen(fd_, SOMAXCONN) < 0, "socket listen error");
    // 非阻塞
    fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK);

    channel_ = std::make_unique<Channel>(fd_);
    channel_->setReadCb(std::bind(&Acceptor::acceptConnection, this));
    channel_->enableRead();
    loop_->addChannel(channel_.get());
    std::cout << "Acceptor listen at " << ip << ":" << port << std::endl;
}

Acceptor::~Acceptor()
{
    loop_->removeChannel(channel_.get());
    errif(fd_ < 0, "Invalid socket");
    ::close(fd_);
}

// 接受客户端连接
void Acceptor::acceptConnection() const
{
    std::cout << "Accept a connection" << std::endl;
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    int clientFd = ::accept4(fd_, (sockaddr*)&clientAddr, &clientAddrLen, SOCK_NONBLOCK | SOCK_CLOEXEC);
    errif(clientFd < 0, "accept error");
    // 交给server建立连接
    if (newConnectionCb_)
        newConnectionCb_(clientFd);
}

void Acceptor::setNewConnectionCb(std::function<void(int)> cb)
{
    newConnectionCb_ = std::move(cb);
}