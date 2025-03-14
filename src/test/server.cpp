#include "server.h"
#include "connection.h"
#include <iostream>
#include <memory>

int main()
{
    std::unique_ptr<Server> server(new Server());
    server->setReadCb([](Connection* conn) {
        // 业务逻辑
        std::string data = conn->read();
        std::cout << data << std::endl;
        conn->send(data);
    });
    server->start();
    return 0;
}