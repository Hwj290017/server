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
    void bind(const InetAddress& addr) const;
    Socket accept(InetAddress& clientAddr) const;
    void nonBlocking() const;
    // void connect(const InetAddress& addr);

    static Socket createNonBlocking(const InetAddress& addr);
    // static Socket createEventSocket();
    // static Socket createTimerSocket();
};

#endif