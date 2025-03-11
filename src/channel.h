#ifndef CHANNEL_H
#define CHANNEL_H
#include "socket.h"
#include <cstdint>
#include <functional>
class EventLoop;
class Channel
{
  private:
    EventLoop* ep;
    Socket* sock;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;
    std::function<void()> callback;

  public:
    Channel(EventLoop* ep, Socket* sock);
    ~Channel();

    void enableReading();

    int getFd() const;
    uint32_t getEvents() const;
    uint32_t getRevents() const;
    bool getInEpoll() const;
    void setInEpoll();

    // void setEvents(uint32_t);
    void setRevents(uint32_t);
    void handleEvent();
    void setCallback(std::function<void()>);
};
#endif