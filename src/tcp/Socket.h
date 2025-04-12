#ifndef SOCKET_H
#define SOCKET_H
#include "InetAddress.h"
#include "RWAbleFd.h"
#include <utility>

class InetAddress;
class Socket : public RWAbleFd
{
  public:
    Socket();
    Socket(int fd) : RWAbleFd(fd)
    {
    }
    Socket(Socket&& other) : RWAbleFd(std::move(other))
    {
    }
    ~Socket() = default;

    void listen() const;
    // 用于服务端接受连接
    Socket accept(InetAddress& clientAddr) const;
    // 用于客户端连接服务器
    void connect(const InetAddress& addr);
    static Socket createServerSocket(const InetAddress& addr);
    static Socket createNonBlocking();
    static Socket createBlocking();
    // static Socket createEventSocket();
    // static Socket createTimerSocket();
};

#endif