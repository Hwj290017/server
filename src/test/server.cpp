#include "TcpConnection.h"
#include "TcpServer.h"
#include "buffer.h"
#include "timer.h"
#include <iostream>
#include <memory>

int main()
{
    InetAddress addr(LOCALHOST, PORT);
    std::unique_ptr<TcpServer> server(new TcpServer(addr));
    server->setMessageCb([](TcpConnection* conn, const Buffer& data, const TimeSpec& time) {
        // 业务逻辑
        std::cout << "Receive data from " << conn->getId() << ": ";
        std::cout.write(data.begin(), data.size());
        std::cout << std::endl;
        conn->send(data.begin(), data.size());
        conn->closeAfter(10);
    });
    server->setOnConnectionCb([](TcpConnection* conn) {
        if (conn->getState() == TcpConnection::Connected)
            std::cout << "hello," << conn->getPeerAddr().toIpPort() << std::endl;
        else
        {
            std::cout << "bye," << conn->getPeerAddr().toIpPort() << std::endl;
        }
    });
    server->start();
    return 0;
}