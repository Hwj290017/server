
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
  private:
    EventLoop mainLoop_;
    // 子线程池
    LoopThreadPool threadPool_;

    std::unique_ptr<Acceptor> acceptor_;
    // 连接池
    std::map<int, std::unique_ptr<Connection>> connections_;
    // 连接回调函数
    std::function<void(Connection*, const std::string&)> messageCb_;

  public:
    Server(const char* ip = LOCALHOST, int port = PORT);
    ~Server();
    void newConnection(int);
    void closeConnection(int);
    void setMessageCb(const std::function<void(Connection*, const std::string&)>& cb);
    void start();
};
#endif