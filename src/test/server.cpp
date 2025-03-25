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
        std::cout << "Receive data from " << conn->getFd() << ": ";
        std::cout.write(data.begin(), data.size());
        std::cout << std::endl;
        conn->send(data.begin(), data.size());
    });
    server->start();
    return 0;
}