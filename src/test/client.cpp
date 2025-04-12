
#include "InetAddress.h"
#include "Socket.h"
int main()
{
    auto serverAddr = InetAddress("127.0.0.1", 8888);
    auto serverSock = Socket::createBlocking();
    serverSock.connect(serverAddr);
    serverSock.write("hello world", 11);
}