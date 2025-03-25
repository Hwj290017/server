
#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include "RWAbleFd.h"
#include "timer.h"
#include "timerqueue.h"
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

#define MAX_EVENTS 1024
class Poller;

class EventFd : public RWAbleFd
{
  public:
    static EventFd createEventFd();

  private:
    EventFd(int fd) : RWAbleFd(fd)
    {
    }
};

class EventLoop
{
    typedef std::function<void()> Task;

  public:
    EventLoop();
    ~EventLoop();

    // 事件驱动
    void loop();

    // 更新事件状态,包括添加修改删除
    void updateChannel(Channel*) const;
    // 是否有事件
    void hasChannel(const Channel*) const;
    void runInLoop(const Task& task);
    void runInLoop(Task&& task);
    void queueInLoop(const Task& task);
    void queueInLoop(Task&& task);
    bool isLoopThread() const;
    void wakeup();
    // time是时间点，timespec_get是获取当前时间点
    TimerId runAt(const Task& task, const TimeSpec& when);
    // delay秒后启动一次task
    TimerId runAfter(const Task& task, double delay);
    // 每隔interval秒启动一次task
    TimerId runEvery(const Task& task, double interval);
    // 移除定时器
    void removeTimer(TimerId& timerId);

  private:
    enum LoopState
    {
        HandlingEvents,
        CallingTasks,
        Waiting
    };

    EventFd wakeupFd_;
    Channel wakeupChannel_;
    std::unique_ptr<Poller> poller_;
    TimerQueue timerQueue_;
    // 任务队列，每次loop中执行
    // 统一运行，和vector一样，后续更换
    std::queue<Task> tasks_;
    std::thread::id threadId_;
    std::mutex mutex_;
    LoopState loopState_;
    // 唤醒时的回调函数
    void handleRead();
};

#endif