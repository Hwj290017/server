
#include "socket.h"
#include <cstdint>
#include <cstdio>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

InetAddress::InetAddress() : addr_len(sizeof(sockaddr_in))
{
}
InetAddress::InetAddress(const char* ip, uint16_t port) : addr_len(sizeof(sockaddr_in))
{
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
}
InetAddress::~InetAddress()
{
}
