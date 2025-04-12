#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "TcpServer.h"
#include "timer.h"
#include <functional>
#include <utility>
class EventLoop;
class InetAddress;

class TcpConnection;
class Buffer;
class HttpRequest;
class HttpResponse;
class HttpServer
{
  public:
    typedef std::function<void(const HttpRequest&, HttpResponse&)> HttpCallback;

    HttpServer(const InetAddress& listenAddr, const std::string& name = "HttpServer");

    // EventLoop* getLoop() const
    // {
    //     return server_.getLoop();
    // }

    /// Not thread safe, callback be registered before calling start().

    template <typename T> void setHttpCallback(T&& cb)
    {
        httpCb_ = std::forward<T>(cb);
    }

    // void setThreadNum(int numThreads)
    // {
    //     server_.setThreadNum(numThreads);
    // }

    void start();

  private:
    void onConnection(TcpConnection* conn);
    void onMessage(TcpConnection* conn, const Buffer& buf, const TimeSpec& receiveTime);
    void onRequest(TcpConnection*, const HttpRequest&);

    TcpServer server_;
    HttpCallback httpCb_;
    std::string name_;
};

#endif // HTTPSERVER_H
