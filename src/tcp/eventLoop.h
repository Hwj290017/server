
#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <functional>
#include <memory>
#include <queue>

#define MAX_EVENTS 1024
class Poller;
class Channel;
class EventLoop
{
  private:
    std::unique_ptr<Poller> poller_;
    // 任务队列，每次loop中执行
    std::queue<std::function<void()>> tasks_;

  public:
    EventLoop();
    ~EventLoop();

    // 事件驱动
    void loop();
    void addChannel(Channel*) const;
    void updateChannel(Channel*) const;
    void removeChannel(Channel*) const;
    void runInLoop(std::function<void()> task);
};
#endif