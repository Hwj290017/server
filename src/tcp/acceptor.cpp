#include "acceptor.h"
#include "channel.h"
#include "util.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <memory>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

Acceptor::Acceptor(const EventLoop* loop, const char* ip, int port) : fd(-1), loop(loop), newConnectionCb([](int) {})
{
    // 地址
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    // 创建socket
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "Socket create error");
    errif(bind(fd, (sockaddr*)&addr, sizeof(addr)) == -1, "Socket bind error");
    errif(::listen(fd, SOMAXCONN) == -1, "socket listen error");
    // 非阻塞
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);

    channel = std::make_unique<Channel>(fd, loop);
    channel->setReadCb(std::bind(&Acceptor::acceptConnection, this));
    channel->enableRead();
}

Acceptor::~Acceptor()
{
    channel->close();
    if (fd != -1)
    {
        close(fd);
        fd = -1;
    }
}

// 接受客户端连接
void Acceptor::acceptConnection() const
{
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    int clientFd = ::accept4(fd, (sockaddr*)&clientAddr, &clientAddrLen, SOCK_NONBLOCK | SOCK_CLOEXEC);
    errif(clientFd == -1, "accept error");
    // 交给server建立连接
    newConnectionCb(clientFd);
}

void Acceptor::setNewConnectionCb(std::function<void(int)> cb)
{
    newConnectionCb = std::move(cb);
}