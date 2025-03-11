#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include <functional>

#define PORT 8888
#define LOCALHOST "127.0.0.1"
class EventLoop;
class Socket;
class InetAddress;
class Channel;
class Acceptor
{
  private:
    EventLoop* loop;
    Socket* sock;
    Channel* acceptChannel;
    std::function<void(Socket*)> newConnectionCb;

  public:
    Acceptor(EventLoop* loop);
    ~Acceptor();
    void acceptConnection() const;
    void setNewConnectionCb(std::function<void(Socket*)>);
};

#endif