
#include "InetAddress.h"
#include "Socket.h"
#include <iostream>
#include <unistd.h>

#define BUFFERLEN 20
int main()
{
    auto serverAddr = InetAddress("127.0.0.1", 10009);
    auto serverSock = Socket::createBlocking();
    auto buf = new char[BUFFERLEN];
    serverSock.connect(serverAddr);
    for (int i = 0; i < BUFFERLEN; i++)
        buf[i] = 'h';

    auto writeNum = serverSock.write(buf, BUFFERLEN);
    std::cout << writeNum << std::endl;

    auto readNum = serverSock.read(buf, BUFFERLEN);

    std::cout << readNum << std::endl;
}