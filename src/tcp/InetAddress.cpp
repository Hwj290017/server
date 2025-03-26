#include "InetAddress.h"
#include <arpa/inet.h>

InetAddress::InetAddress(const char* ip, uint16_t port) : addrLen_(sizeof(addr_))
{
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = inet_addr(ip);
    addr_.sin_port = htons(port);
}
