#pragma once

#include <cstdint>
#include <netinet/in.h>
#include <string>
namespace tcp
{
class InetAddress
{
  public:
    InetAddress(const char* ip, uint16_t port);
    InetAddress(const InetAddress& other) = default;
    InetAddress() : addrLen_(sizeof(addr_))
    {
    }
    ~InetAddress() = default;
    std::string toIpPort() const;

    sockaddr_in addr_;
    socklen_t addrLen_;
};

} // namespace tcp
