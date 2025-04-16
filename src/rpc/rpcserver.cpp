#include "rpcserver.h"
#include "ConnectionType.h"
#include "TcpConnection.h"
#include "buffer.h"

#include "rpccontext.h"
#include "service.pb.h"
#include "tcpserver.h"
#include "timer.h"
#include <any>
#include <functional>

RpcServer::RpcServer(const InetAddress& listenAddr) : server_(listenAddr, "RpcServer")
{
    server_.setOnConnectionCb([](TcpConnection* conn) {
        if (conn->getState() == TcpConnection::State::Connected)
        {
            conn->setContext(RpcContext());
        }
        else
        {
        }
    });

    server_.setMessageCb([this](TcpConnection* conn, const Buffer& buffer, const TimeSpec& time) {
        auto& context = std::any_cast<RpcContext&>(conn->getContext());
        auto request = context.parseRequset(buffer);
        if (context.isRight())
        {
            auto response = handler_.call(request);
            conn->send(response.SerializeAsString());
        }
    });
}

RpcServer::~RpcServer() = default;

void RpcServer::start()
{
    server_.start();
}

void RpcServer::stop()
{
}
