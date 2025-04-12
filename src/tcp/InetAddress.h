#ifndef INETADDRESS_H
#define INETADDRESS_H

#include <cstdint>
#include <netinet/in.h>
#include <string>
class InetAddress
{
  public:
    InetAddress(const char* ip, uint16_t port);
    InetAddress() : addrLen_(sizeof(addr_))
    {
    }
    ~InetAddress() = default;
    std::string toIpPort() const;

    sockaddr_in addr_;
    socklen_t addrLen_;
};

#endif // INETADDRESS_H
