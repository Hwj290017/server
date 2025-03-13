#ifndef CHANNEL_H
#define CHANNEL_H

#include "socket.h"
#include <cstdint>
#include <functional>
class EventLoop;
class Channel
{
  private:
    int fd;
    const EventLoop* ep;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;
    std::function<void()> cb;

  public:
    Channel(int fd, const EventLoop* loop);
    ~Channel();

    void enableReading();

    int getFd() const;
    uint32_t getEvents() const;
    uint32_t getRevents() const;
    bool getInEpoll() const;
    void setInEpoll();

    void setRevents(uint32_t);
    void handleEvent() const;
    void setCallback(std::function<void()>);
};
#endif