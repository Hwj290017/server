#include "InetAddress.h"
#include <arpa/inet.h>

InetAddress::InetAddress(const char* ip, uint16_t port) : addrLen_(sizeof(addr_))
{
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = inet_addr(ip);
    addr_.sin_port = htons(port);
}

std::string InetAddress::toIpPort() const

{
    return inet_ntoa(addr_.sin_addr) + std::string(":") + std::to_string(ntohs(addr_.sin_port));
}
