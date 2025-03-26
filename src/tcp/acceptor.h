#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "InetAddress.h"
#include "Socket.h"
#include "channel.h"
#include <functional>

class EventLoop;
class Acceptor
{
    using NewConnectionCb = std::function<void(Socket&&, const InetAddress&)>;

  public:
    Acceptor(EventLoop* loop, const InetAddress& addr);
    ~Acceptor();

    // set a callback to be called when a new connection is accepted
    void handleRead() const;

    template <typename T> void setNewConnectionCb(T&& cb)
    {
        newConnectionCb_ = std::forward<T>(cb);
    }

  private:
    EventLoop* loop_;
    Socket socket_;
    Channel channel_;
    InetAddress addr_;

    NewConnectionCb newConnectionCb_;
};

#endif