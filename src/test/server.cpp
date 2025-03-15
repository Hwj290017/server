#include "server.h"
#include "connection.h"
#include <iostream>
#include <memory>

int main()
{
    std::unique_ptr<Server> server(new Server());
    server->setMessageCb([](Connection* conn, const std::string& data) {
        // 业务逻辑
        if (data.empty())
            std::cout << "Now receive data from client: " << std::endl;
        std::cout << data << std::endl;
        conn->send(data);
    });
    server->start();
    return 0;
}