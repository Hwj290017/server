
#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <functional>
#include <queue>
#define MAX_EVENTS 1024

class Channel;
class epoll_event;
class EventLoop
{
  private:
    int epfd;
    bool quit;
    epoll_event* events;
    std::queue<std::function<void()>> tasks;

  public:
    EventLoop();
    ~EventLoop();
    void loop();
    void updateChannel(Channel*) const;
    void closeChannel(const Channel*) const;
    void runInLoop(std::function<void()> cb);
};
#endif