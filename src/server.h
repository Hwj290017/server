
#ifndef SERVER_H
#define SERVER_H

#include <functional>
#include <map>
#include <vector>

#define THREADNUM std::thread::hardware_concurrency()

class EventLoop;
class Acceptor;
class Connection;
class Socket;
class ThreadPool;

class Server
{
  private:
    EventLoop* mainReactor;                 // 只负责接受连接，然后分发给一个subReactor
    Acceptor* acceptor;                     // 连接接受器
    std::map<int, Connection*> connections; // TCP连接
    std::vector<EventLoop*> subReactors;    // 负责处理事件循环
    ThreadPool* thpool;                     // 线程池
    std::function<void(Connection*)> onConnectionCb;

  public:
    Server(EventLoop*);
    ~Server();
    void newConnection(Socket* severSock);
    void deleteConnection(Socket* clientScok);
    void onConnection(std::function<void(Connection*)> Cb);
};
#endif