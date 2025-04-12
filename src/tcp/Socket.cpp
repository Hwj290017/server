#include "Socket.h"
#include "InetAddress.h"
#include "util.h"
#include <fcntl.h>
#include <sys/socket.h>

void Socket::listen() const
{
    errif(::listen(fd_, SOMAXCONN) < 0, "listen error");
}

void Socket::connect(const InetAddress& addr)
{
    errif(::connect(fd_, (sockaddr*)&addr.addr_, addr.addrLen_) < 0, "connect error");
}

Socket Socket::accept(InetAddress& addr) const
{
    auto clientFd = ::accept4(fd_, (sockaddr*)&addr.addr_, &addr.addrLen_, SOCK_NONBLOCK | SOCK_CLOEXEC);

    errif(clientFd < 0, "accept error");
    return Socket(clientFd);
}

Socket Socket::createServerSocket(const InetAddress& addr)
{
    auto sock = createNonBlocking();
    errif(::bind(sock.fd_, (sockaddr*)&addr.addr_, addr.addrLen_) < 0, "bind error");
    return sock;
}

Socket Socket::createNonBlocking()
{
    auto fd = ::socket(AF_INET, SOCK_STREAM, 0);
    errif(fd < 0, "create socket error");
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
    return Socket(fd);
}

Socket Socket::createBlocking()
{
    auto fd = ::socket(AF_INET, SOCK_STREAM, 0);
    errif(fd < 0, "create socket error");
    return Socket(fd);
}