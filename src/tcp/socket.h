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

// 返回读字节数，无数据返回0，断开连接返回-1
inline std::size_t readNoBlock(int fd, void* buf, size_t len)
{
    return ::read(fd, buf, len);
}

inline std::size_t writeNoBlock(int fd, const void* buf, size_t len)
{
    std::size_t writeNumTotal = 0;
    while (len > 0)
    {
        auto writeNum = ::write(fd, buf, len);
        if (writeNum <= 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                return writeNumTotal;
            }
            else
                return -1;
        }
        else
        {
            writeNumTotal += writeNum;
            buf = (char*)buf + writeNum;
            len -= writeNum;
        }
    }
    return writeNumTotal;
}
} // namespace socket

} // namespace tcp
