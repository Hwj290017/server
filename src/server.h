
#ifndef SERVER_H
#define SERVER_H

#include "LoopThreadPool.h"
#include "eventLoop.h"
#include <functional>
#include <map>
#include <memory>
#include <vector>

#define PORT 8888
#define LOCALHOST "127.0.0.1"

class Acceptor;
class Connection;
class ThreadPool;

class Server
{
  private:
    const EventLoop mainLoop_;
    LoopThreadPool threadPool_;
    // 只负责接受连接，然后分发给一个subReactor
    std::unique_ptr<Acceptor> acceptor_;                     // 连接接受器
    std::map<int, std::unique_ptr<Connection>> connections_; // TCP连接 // 线程池
    std::function<void(Connection*)> onConnectionCb;

  public:
    Server(const char* ip = LOCALHOST, int port = PORT);
    ~Server();
    void newConnection(int);
    void deleteConnection(int);
    void onConnection(std::function<void(Connection*)> cb);
    void start();
};
#endif