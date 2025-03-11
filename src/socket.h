
#ifndef SOCKET_H
#define SOCKET_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
class InetAddress
{
  public:
    sockaddr_in addr;
    socklen_t addr_len;
    InetAddress();
    InetAddress(const char* ip, uint16_t port);
    InetAddress(const InetAddress&) = delete;
    InetAddress(InetAddress&&) = delete;
    ~InetAddress();
};

class Socket
{
  private:
    int fd;

  public:
    Socket();
    Socket(int);
    Socket(const Socket&) = delete;
    Socket(Socket&&);
    ~Socket();

    int getFd() const;
    void bind(InetAddress*) const;
    void listen() const;
    void setnonblocking() const;

    Socket accept(InetAddress*) const;
};

#endif
