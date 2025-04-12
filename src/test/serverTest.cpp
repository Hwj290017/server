

#include "InetAddress.h"
#include "Socket.h"
#include "acceptor.h"
#include "eventLoop.h"
#include "log.h"
#include <iostream>
int main()
{
    auto serverAddr = InetAddress("127.0.0.1", 8888);
    auto loop = EventLoop();
    auto acceptor = Acceptor(&loop, serverAddr);
    loop.loop();
    // auto clientAddr = InetAddress();
    // auto clientSock = serverSock.accept(clientAddr);
    // Logger::logger << clientAddr.toIpPort();
    // std::string msg = "Hello, world!";
    // clientSock.write(msg.data(), msg.size());
    return 0;
}