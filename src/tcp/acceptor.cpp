#include "acceptor.h"
#include "channel.h"
#include "eventLoop.h"
#include "util.h"
#include <arpa/inet.h>
#include <cassert>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

Acceptor::Acceptor(EventLoop* loop, const char* ip, int port) : loop_(loop), fd_(createSocket(ip, port)), channel_(fd_)
{
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    errif(bind(fd_, (sockaddr*)&addr, sizeof(addr)) < 0, "Socket bind error");
    errif(::listen(fd_, SOMAXCONN) < 0, "socket listen error");
    // 非阻塞
    fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK);
    channel_.setReadCb(std::bind(&Acceptor::acceptConnection, this));
    channel_.enableRead();
    channel_.start();
    loop_->updateChannel(&channel_);
    std::cout << "Acceptor listen at " << ip << ":" << port << std::endl;
}

Acceptor::~Acceptor()
{
    loop_->updateChannel(&channel_);
    assert(fd_ >= 0);
    ::close(fd_);
}

// 接受客户端连接
void Acceptor::acceptConnection() const
{
    std::cout << "Accept a connection" << std::endl;
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    int clientFd = ::accept4(fd_, (sockaddr*)&clientAddr, &clientAddrLen, SOCK_NONBLOCK | SOCK_CLOEXEC);
    // 交给server建立连接
    if (clientFd >= 0 && newConnectionCb_)
        newConnectionCb_(clientFd);
}

int Acceptor::createSocket(const char* ip, int port)
{
    // 创建socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    assert(fd >= 0);
    return fd;
}