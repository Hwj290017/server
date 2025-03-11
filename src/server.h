
#ifndef SERVER_H
#define SERVER_H

#include <functional>
#include <map>
#include <memory>
#include <vector>

#define THREADNUM std::thread::hardware_concurrency()
#define PORT 8888
#define LOCALHOST "127.0.0.1"

class EventLoop;
class Acceptor;
class Connection;
class ThreadPool;

class Server
{
  private:
    std::unique_ptr<EventLoop> mainReactor;                 // 只负责接受连接，然后分发给一个subReactor
    std::unique_ptr<Acceptor> acceptor;                     // 连接接受器
    std::map<int, std::unique_ptr<Connection>> connections; // TCP连接
    std::vector<std::unique_ptr<EventLoop>> subReactors;    // 负责处理事件循环
    std::unique_ptr<ThreadPool> thpool;                     // 线程池
    std::function<void(Connection*)> onConnectionCb;

  public:
    Server(const char* ip = LOCALHOST, int port = PORT);
    ~Server();
    void newConnection(int);
    void deleteConnection(int);
    void onConnection(std::function<void(Connection*)> cb);
    void start() const;
};
#endif