#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "InetAddress.h"
#include "Socket.h"
#include "channel.h"
#include <functional>

class EventLoop;
class Acceptor
{
    using CallBack = std::function<void(Socket&&, const InetAddress&)>;

  public:
    Acceptor(EventLoop* loop, const InetAddress& addr);
    ~Acceptor();
    void handleRead() const;
    void setNewConnectionCb(const CallBack& cb)
    {
        newConnectionCb_ = cb;
    }

  private:
    EventLoop* loop_;
    Socket socket_;
    Channel channel_;
    InetAddress addr_;

    CallBack newConnectionCb_;
};

#endif