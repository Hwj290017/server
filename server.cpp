#include "src/server.h"
#include "src/connection.h"
#include "src/eventLoop.h"
#include <iostream>

int main()
{
    EventLoop* loop = new EventLoop();
    Server* server = new Server(loop);
    server->onConnection([](Connection* conn) {
        // 业务逻辑
        conn->read();
        std::cout << conn->getReadBuffer().c_str() << std::endl;
    });
    loop->loop(); // 开始事件循环
    delete server;
    delete loop;
    return 0;
}