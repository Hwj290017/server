
#ifndef SERVER_H
#define SERVER_H

#include "LoopThreadPool.h"
#include "eventLoop.h"
#include <functional>
#include <map>
#include <memory>
#include <string>

#define PORT 8888
#define LOCALHOST "127.0.0.1"

class Acceptor;
class Connection;
class ThreadPool;

class Server
{
    typedef std::function<void(Connection*, const std::string&)> MessageCb;
    typedef std::unique_ptr<Connection> ConnectionPtr;
    typedef std::unique_ptr<Acceptor> AcceptorPtr;

  public:
    Server(const char* ip = LOCALHOST, int port = PORT);
    ~Server();
    void start();
    void setMessageCb(const MessageCb& cb)
    {
        messageCb_ = cb;
    }
    void setMessageCb(MessageCb&& cb)
    {
        messageCb_ = std::move(cb);
    }

  private:
    EventLoop mainLoop_;
    // 子线程池
    LoopThreadPool threadPool_;
    // 接收器
    AcceptorPtr acceptor_;
    // 连接池
    std::map<int, ConnectionPtr> connections_;
    // 连接回调函数
    MessageCb messageCb_;

    void newConnection(int);
    void closeConnection(int);
};
#endif