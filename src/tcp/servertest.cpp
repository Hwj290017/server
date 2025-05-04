#include "tcp/server.h"
#include "tcp/acceptor.h"
#include "tcp/connection.h"
#include "tcp/inetaddress.h"
#include "tcp/tempptr.h"
#include <functional>
#include <iostream>
int main()
{
    tcp::InetAddress serverAdrr("127.0.0.1", 8888);
    tcp::Server server;
    tcp::Acceptor::Tasks tasks;
    tasks.startTask = [](tcp::TempPtr<tcp::Acceptor> acceptor) {};
    // tasks.acceptTask = [&server](tcp::TempPtr<tcp::Acceptor> acceptor, int clientfd, const tcp::InetAddress&
    // peerAddr) {
    //     tcp::Connection::MessageTask readTask;
    // };
    server.newAcceptor(serverAdrr, tasks);
    std::function<void(int)> task;
    task = [&server](int fd) {};

    server.start();
}