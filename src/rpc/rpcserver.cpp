#include "rpcserver.h"
#include "TcpConnection.h"
RpcServer::RpcServer(const InetAddress& listenAddr) : server_(listenAddr, "RpcServer")
{
    server_.setOnConnectionCb([](TcpConnection* conn) {
        if (conn->getState() == TcpConnection::State::Connected)
        {
            conn->setContext()
        }
    });
}