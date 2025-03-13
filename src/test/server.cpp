#include "server.h"
#include "connection.h"
#include <iostream>
#include <memory>

int main()
{
    std::unique_ptr<Server> server(new Server());
    server->onConnection([](Connection* conn) {
        // 业务逻辑
        conn->read();
        std::cout << conn->getReadBuffer().c_str() << std::endl;
    });
    server->start();
    return 0;
}