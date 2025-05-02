#pragma once
#include "EventLoop.h"
#include "InetAddress.h"
class Connector
{
  public:
    Connector(const InetAddress& serverAddr)
    {
    }
    ~Connector();

  private:
    int fd_;
    InetAddress serverAddr_;
    EventLoop* loop_;
    Channel channel_;
    void handleRead();
    void handleWrite();
};