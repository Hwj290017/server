#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <functional>
#include <memory>

class EventLoop;
class InetAddress;
class Channel;
class Acceptor
{
  private:
    int fd;
    const EventLoop* loop;
    std::unique_ptr<Channel> channel;
    std::function<void(int)> newConnectionCb;

  public:
    Acceptor(const EventLoop* loop, const char* ip, int port);
    ~Acceptor();
    void acceptConnection() const;
    void setNewConnectionCb(std::function<void(int)>);
};

#endif