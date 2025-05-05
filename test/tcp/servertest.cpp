#include "tcp/server.h"
#include "tcp/acceptor.h"
#include "tcp/connection.h"
#include "tcp/inetaddress.h"
#include "tcp/tempptr.h"
#include <functional>
#include <iostream>
#include <unistd.h>
int main()
{
    tcp::InetAddress serverAdrr("127.0.0.1", 8888);
    tcp::Server server;
    tcp::Acceptor::Tasks tasks;
    tasks.startTask = [](const tcp::TempPtr<tcp::Acceptor>& acceptor) { std::cout << "Acceptor start" << std::endl; };
    tasks.acceptTask = [&server](const tcp::TempPtr<tcp::Acceptor>& acceptor, int clientfd,
                                 const tcp::InetAddress& peerAddr) {
        tcp::Connection::Tasks tasks;
        tasks.startTask = [](const tcp::TempPtr<tcp::Connection>& connection) {
            std::cout << "Connection start" << std::endl;
        };
        tasks.messageTask = [](const tcp::TempPtr<tcp::Connection>& connection, const void* data, std::size_t size) {
            std::cout << "Received message: " << std::string(static_cast<const char*>(data), size) << std::endl;
            connection->send(data, size);
        };
        tasks.stopTask = [](const tcp::TempPtr<tcp::Connection>& connection) {
            std::cout << "Connection stop" << std::endl;
        };
        server.newConnection(clientfd, peerAddr, tasks);
    };
    server.newAcceptor(serverAdrr, tasks);
    // std::function<void(int)> task;
    // task = [&server](int fd) {};

    server.start();
    sleep(50);
}