#ifndef INETADDRESS_H
#define INETADDRESS_H

#include <cstdint>
#include <netinet/in.h>
class InetAddress
{
  public:
    InetAddress(const char* ip, uint16_t port);
    InetAddress()
    {
    }
    ~InetAddress() = default;

    sockaddr_in addr_;
    socklen_t addrLen_;
};

#endif // INETADDRESS_H
