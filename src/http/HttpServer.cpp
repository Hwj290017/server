#include "HttpServer.h"
#include "TcpConnection.h"
#include "httpContext.h"
#include "httpResponse.h"
#include <any>
#include <cassert>
#include <functional>

HttpServer::HttpServer(const InetAddress& listenAddr, const std::string& name) : server_(listenAddr, name)
{
    server_.setOnConnectionCb(std::bind(&HttpServer::onConnection, this, std::placeholders::_1));
    server_.setMessageCb(
        std::bind(&HttpServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void HttpServer::start()
{
    server_.start();
}

void HttpServer::onConnection(TcpConnection* conn)
{
    assert(conn->getState() == TcpConnection::Connected);
    conn->setContext(HttpContext());
}

void HttpServer::onMessage(TcpConnection* conn, const Buffer& buf, const TimeSpec& receiveTime)
{
    auto& context = std::any_cast<HttpContext&>(conn->getContext());

    if (!context.parseRequest(buf, receiveTime))
    {
        conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
        conn->close();
    }

    if (context.gotAll())
    {
        onRequest(conn, context.request());
        context.reset();
    }
}

void HttpServer::onRequest(TcpConnection* conn, const HttpRequest& req)
{
    const std::string& connection = req.getHeader("Connection");
    bool close = connection == "close" || (req.version() == HttpRequest::Http10 && connection != "Keep-Alive");
    HttpResponse response(close);
    httpCb_(req, response);
    Buffer buf;
    response.message(buf);
    conn->send(buf.begin(), buf.size());
    if (response.closeConnection())
    {
        conn->close();
    }
}
