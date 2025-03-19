#include "server.h"
#include "connection.h"
#include <iostream>
#include <memory>

int main()
{
    std::unique_ptr<Server> server(new Server());
    server->setMessageCb([](Connection* conn, const std::string& data) {
        // 业务逻辑
        std::cout << "Receive data from " << conn->getSock() << ": " << data << std::endl;
    });
    server->start();
    return 0;
}