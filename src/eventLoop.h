
#ifndef EVENTLOOP_H
#define EVENTLOOP_H
class Epoll;
class ThreadPool;
class Channel;
class EventLoop
{
  private:
    Epoll* ep;
    bool quit;

  public:
    EventLoop();
    ~EventLoop();
    void loop() const;
    void updateChannel(Channel*) const;
};
#endif