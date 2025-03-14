
#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#define MAX_EVENTS 1024

class Channel;
class epoll_event;
class EventLoop
{
  private:
    int epfd;
    bool quit;
    epoll_event* events;

  public:
    EventLoop();
    ~EventLoop();
    void loop() const;
    void updateChannel(Channel*) const;
    void closeChannel(const Channel*) const;
};
#endif