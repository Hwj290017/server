#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "channel.h"
#include <functional>
class EventLoop;
class Acceptor
{
  private:
    EventLoop* loop_;
    int fd_;
    Channel channel_;

    std::function<void(int)> newConnectionCb_;

    static int createSocket(const char* ip, int port);

  public:
    Acceptor(EventLoop* loop, const char* ip, int port);
    ~Acceptor();
    void acceptConnection() const;
    void setNewConnectionCb(const std::function<void(int)>& cb)
    {
        newConnectionCb_ = cb;
    }
};

#endif