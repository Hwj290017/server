#include "Socket.h"
#include "InetAddress.h"
#include "util.h"
#include <fcntl.h>

void Socket::bind(const InetAddress& addr) const
{
    errif(::bind(fd_, (sockaddr*)&addr.addr_, addr.addrLen_) < 0, "bind error");
}

void Socket::listen() const
{
    errif(::listen(fd_, SOMAXCONN) < 0, "listen error");
}

// void Socket::connect(const InetAddress& addr)
// {
// }

Socket Socket::accept(InetAddress& addr) const
{
    auto clientFd = ::accept4(fd_, (sockaddr*)&addr.addr_, &addr.addrLen_, SOCK_NONBLOCK | SOCK_CLOEXEC);
    errif(clientFd < 0, "accept error");
    return Socket(clientFd);
}

void Socket::nonBlocking() const
{
    fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK);
}

Socket Socket::createNonBlocking(const InetAddress& addr)
{
    auto fd = ::socket(AF_INET, SOCK_STREAM, 0);
    errif(fd < 0, "create socket error");
    Socket sock(fd);
    sock.nonBlocking();
    return sock;
}
