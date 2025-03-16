#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <functional>
#include <memory>
class Channel;
class EventLoop;
class Acceptor
{
  private:
    int fd_;
    EventLoop* loop_;
    std::unique_ptr<Channel> channel_;
    std::function<void(int)> newConnectionCb_;

  public:
    Acceptor(EventLoop* loop, const char* ip, int port);
    ~Acceptor();
    void acceptConnection() const;
    void setNewConnectionCb(std::function<void(int)>);
};

#endif