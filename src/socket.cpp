
#include "socket.h"
#include "util.h"
#include <cstdint>
#include <cstdio>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

InetAddress::InetAddress() : addr_len(sizeof(sockaddr_in))
{
}
InetAddress::InetAddress(const char* ip, uint16_t port) : addr_len(sizeof(sockaddr_in))
{
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
}
InetAddress::~InetAddress()
{
}
Socket::Socket(int fd) : fd(fd)
{
    errif(fd == -1, "socket create error");
}
Socket::Socket()
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "socket create error");
}

Socket::Socket(Socket&& other) : fd(other.fd)
{
    // printf("%d\n", fd);
    other.fd = -1;
}
Socket::~Socket()
{
    if (fd != -1)
        close(fd);
}

int Socket::getFd() const
{
    return fd;
}
void Socket::bind(InetAddress* addr) const
{
    errif(::bind(fd, (sockaddr*)&addr->addr, addr->addr_len) == -1, "Socket bind error");
}
void Socket::listen() const
{
    errif(::listen(fd, SOMAXCONN) == -1, "socket listen error");
}

void Socket::setnonblocking() const
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

Socket Socket::accept(InetAddress* addr) const
{
    int clientFd = ::accept(fd, (sockaddr*)&addr->addr, &addr->addr_len);
    errif(clientFd == -1, "socket accept error");
    return Socket(clientFd);
}
