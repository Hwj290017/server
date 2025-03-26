
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
    using Task = std::function<void()>;

  public:
    EventLoop();
    ~EventLoop();

    // 事件驱动
    void loop();

    // 更新事件状态,包括添加修改删除
    void updateChannel(Channel*) const;
    // 是否有事件
    void hasChannel(const Channel*) const;
    template <typename T> void runInLoop(T&& task)
    {
        if (isLoopThread())
            task();
        else
            queueInLoop(std::forward<T>(task));
    }

    template <typename T> void queueInLoop(T&& task)
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            tasks_.push(task);
        }
        // 无论是不是当前线程，只要不是在处理事件都唤醒
        if (loopState_ != HandlingEvents)
            wakeup();
    }

    bool isLoopThread() const;
    void wakeup();
    // time是时间点，timespec_get是获取当前时间点
    TimerId runAt(const Task& task, const TimeSpec& when);
    // delay秒后启动一次task
    TimerId runAfter(const Task& task, double delay);
    // 每隔interval秒启动一次task
    TimerId runEvery(const Task& task, double interval);
    // 更新定时器
    void updateTimer(const TimerId& timerId);
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