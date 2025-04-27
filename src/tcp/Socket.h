#pragma once

#include "InetAddress.h"
#include <cassert>
#include <fcntl.h>
#include <sys/socket.h>
namespace tcp
{
namespace Socket
{

// 用于服务端监听连接
inline void listen(int acceptorFd, int backlog)
{
    assert(::listen(acceptorFd, SOMAXCONN) >= 0);
}
inline int accept(int listenFd, InetAddress* clientAddr)
{
    int clientFd = ::accept(listenFd, (sockaddr*)&clientAddr->addr_, &clientAddr->addrLen_);
    assert(clientFd >= 0);
    return clientFd;
}
// 用于客户端连接服务端

inline int createAcceptorSocket(const InetAddress& addr)
{
    auto fd = ::socket(AF_INET, SOCK_STREAM, 0);
    assert(fd >= 0);
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
    assert(::bind(fd, (sockaddr*)&addr.addr_, addr.addrLen_) >= 0);
    return fd;
}
inline int createConnectorSocket(const InetAddress& addr)
{
    auto fd = ::socket(AF_INET, SOCK_STREAM, 0);
    assert(fd >= 0);
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
    assert(::connect(fd, (sockaddr*)&addr.addr_, addr.addrLen_) >= 0 || errno == EINPROGRESS);
    return fd;
}
} // namespace Socket

} // namespace tcp
