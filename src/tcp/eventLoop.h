
#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <functional>
#include <memory>
#include <mutex>
#include <queue>

#define MAX_EVENTS 1024
class Poller;
class Channel;
class EventLoop
{
  private:
    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_;

    // 在加入事件到队列时判断是否需要唤醒
    bool needToWakeup_;
    std::unique_ptr<Poller> poller_;
    // 任务队列，每次loop中执行
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    // 唤醒时的回调函数
    void handleRead();

  public:
    EventLoop();
    ~EventLoop();

    // 事件驱动
    void loop();
    void addChannel(Channel*) const;
    void updateChannel(Channel*) const;
    void removeChannel(Channel*) const;
    void runInLoop(const std::function<void()>& task);
    void wakeup();
    static int createEventfd();
};

#endif