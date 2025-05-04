
#include "socket.h"
#include "tcp/inetaddress.h"
#include <iostream>
#include <unistd.h>

#define BUFFERLEN 20
int main()
{
    auto serverAddr = tcp::InetAddress("127.0.0.1", 10009);
    auto serverSock = tcp::socket::createClientSocket(serverAddr);
    auto buf = new char[BUFFERLEN];
    for (int i = 0; i < BUFFERLEN; i++)
        buf[i] = 'h';

    auto writeNum = tcp::socket::writeBlocking(serverSock, buf, BUFFERLEN);
    std::cout << writeNum << std::endl;
    auto readNum = tcp::socket::readBlocking(serverSock, buf, BUFFERLEN);
    std::cout << readNum << std::endl;
}