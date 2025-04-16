#include "InetAddress.h"
#include "TcpConnection.h"
#include "TcpServer.h"
#include "buffer.h"
#include "log.h"
#include "timer.h"
#include <iostream>
int main()
{
    InetAddress serverAdrr("127.0.0.1", 8888);
    TcpServer server(serverAdrr);
    server.setMessageCb([](TcpConnection* conn, const Buffer& buffer, const TimeSpec& time) {
        auto str = buffer.toString();
        std::cout << buffer.size();
        conn->send(str);
    });
    server.start();
}