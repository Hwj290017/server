
#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "ConnectionType.h"
#include "InetAddress.h"
#include "LoopThreadPool.h"
#include "Socket.h"
#include "buffer.h"
#include "eventLoop.h"
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <utility>

#define PORT 8888
#define LOCALHOST "127.0.0.1"

class Acceptor;
class TcpConnection;
class ThreadPool;

class TcpServer
{
    typedef std::function<void(Socket&&, const InetAddress&)> NewConnectionCb;
    typedef std::unique_ptr<TcpConnection> TcpConnectionPtr;
    typedef std::unique_ptr<Acceptor> AcceptorPtr;

  public:
    TcpServer(const InetAddress& addr, const std::string& name = "TcpServer");
    ~TcpServer();
    void start();
    // 建立连接回调
    template <typename T> void setOnConnectionCb(T&& cb)
    {
        onConnectionCb_ = std::forward<T>(cb);
    }

    // 消息处理回调
    template <typename T> void setMessageCb(T&& cb)
    {
        messageCb_ = std::forward<T>(cb);
    }

  private:
    InetAddress localAddr_;
    std::string name_;
    EventLoop mainLoop_;
    // 子线程池
    LoopThreadPool threadPool_;
    // 接收器
    AcceptorPtr acceptor_;
    // 连接池
    std::map<int, TcpConnectionPtr> connections_;
    // 连接建立回调
    OnConnectionCb onConnectionCb_;
    // 消息处理函数
    MessageCb messageCb_;

    void newConnection(Socket&& clientSock, const InetAddress& peerAddr);
    void closeConnection(int);
    void closeConnectionInLoop(int connid);
    static int nextConnId_;
};
#endif