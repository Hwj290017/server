#include "tcp/server.h"
#include "tcp/acceptor.h"
#include "tcp/connection.h"
#include "tcp/inetaddress.h"
#include "util/log.h"
#include <iostream>
int main()
{
    tcp::InetAddress serverAdrr("127.0.0.1", 8888);
    tcp::Server server;
    tcp::Acceptor::Tasks tasks;
    tasks.acceptTask = [&server](tcp::Acceptor* acceptor, const tcp::InetAddress& peerAddr,
                                 tcp::Connection::Tasks* tasks) -> bool {
        tasks->messageTask = [&server](tcp::Connection* conn, const void* data, std::size_t size) {

        };
    };
    server.newAcceptor(serverAdrr, tasks);

    server.start();
}