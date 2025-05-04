#pragma once

#include "tcp/inetaddress.h"
#include <cassert>
#include <cstddef>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
namespace tcp
{
namespace socket
{

// 用于服务端监听连接
inline auto listen(int acceptorFd, int backlog)
{
    assert(::listen(acceptorFd, SOMAXCONN) >= 0);
}
inline auto accept(int listenFd, InetAddress* clientAddr)
{
    int clientFd = ::accept(listenFd, (sockaddr*)&clientAddr->addr_, &clientAddr->addrLen_);
    assert(clientFd >= 0);
    return clientFd;
}
// 用于客户端连接服务端

inline auto createAcceptorSocket(const InetAddress& addr)
{
    auto fd = ::socket(AF_INET, SOCK_STREAM, 0);
    assert(fd >= 0);
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
    assert(::bind(fd, (sockaddr*)&addr.addr_, addr.addrLen_) >= 0);
    return fd;
}
inline auto createConnectorSocket(const InetAddress& addr)
{
    auto fd = ::socket(AF_INET, SOCK_STREAM, 0);
    assert(fd >= 0);
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
    assert(::connect(fd, (sockaddr*)&addr.addr_, addr.addrLen_) >= 0 || errno == EINPROGRESS);
    return fd;
}

inline auto createClientSocket(const InetAddress& serverAddr)
{
    auto fd = ::socket(AF_INET, SOCK_STREAM, 0);
    assert(fd >= 0);
    assert(::connect(fd, (sockaddr*)&serverAddr.addr_, serverAddr.addrLen_) >= 0 || errno == EINPROGRESS);
    return fd;
}
// 返回读字节数，无数据返回0，断开连接返回-1
inline int readNoBlocking(int fd, void* buf, size_t len)
{
    auto readNum = ::read(fd, buf, len);
    if (readNum <= 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return 0;
        }
        else
            return -1;
    }
    return readNum;
}

inline int writeNoBlocking(int fd, const void* buf, size_t len)
{
    auto writeNum = ::write(fd, buf, len);
    if (writeNum <= 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return 0;
        }
        else
            return -1;
    }
    return writeNum;
}

inline auto readBlocking(int fd, void* buf, size_t len)
{
    return ::read(fd, buf, len);
}

inline auto writeBlocking(int fd, const void* buf, size_t len)
{
    return ::write(fd, buf, len);
}
} // namespace socket

} // namespace tcp
