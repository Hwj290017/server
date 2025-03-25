#include "eventLoop.h"
#include "epoller.h"
#include "util.h"
#include <cstdint>
#include <iostream>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <vector>

EventFd EventFd::createEventFd()
{
    int fd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    errif(fd < 0, "Failed to create eventfd");
    return EventFd(fd);
}

EventLoop::EventLoop()
    : wakeupFd_(EventFd::createEventFd()), wakeupChannel_(&wakeupFd_), poller_(new Epoller()), tasks_(),
      timerQueue_(this), loopState_(Waiting), threadId_(std::this_thread::get_id())
{
    wakeupChannel_.setReadCb(std::bind(&EventLoop::handleRead, this));
    wakeupChannel_.enableRead();
    wakeupChannel_.enableEt();
    wakeupChannel_.start();
    poller_->updateChannel(&wakeupChannel_);
}

EventLoop::~EventLoop() = default;

void EventLoop::loop()
{
    while (true)
    {
        std::vector<Channel*> activeChannels;
        std::queue<std::function<void()>> tempTasks;
        // 更新时间队列
        timerQueue_.updateTimerFd();
        // 等待事件
        loopState_ = Waiting;
        poller_->poll(activeChannels, -1);
        std::cout << "activeChannels size: " << activeChannels.size() << std::endl;
        // 处理时间
        loopState_ = HandlingEvents;
        for (auto channel : activeChannels)
        {
            std::cout << "handle event" << channel->fd()->fd() << std::endl;
            channel->handleEvent();
        }

        // 处理任务队列
        loopState_ = CallingTasks;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            tempTasks.swap(tasks_);
        }
        while (!tempTasks.empty())
        {
            std::function<void()>& task = tempTasks.front();
            task();
            tempTasks.pop();
        }
    }
}

void EventLoop::updateChannel(Channel* channel) const
{
    poller_->updateChannel(channel);
}

void EventLoop::runInLoop(const Task& task)
{
    if (isLoopThread())
        task();
    else
        queueInLoop(task);
}

void EventLoop::runInLoop(Task&& task)
{
    if (isLoopThread())
        task();
    else
        queueInLoop(std::move(task));
}

void EventLoop::queueInLoop(const Task& task)
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        tasks_.push(task);
    }
    // 无论是不是当前线程，只要不是在处理事件都唤醒
    if (loopState_ != HandlingEvents)
        wakeup();
}
void EventLoop::queueInLoop(Task&& task)
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        tasks_.push(std::move(task));
    }
    if (loopState_ != HandlingEvents)
        wakeup();
}

// 唤醒事件回调，读取数据
void EventLoop::handleRead()
{
    uint64_t count = 1;
    int n = wakeupFd_.read(reinterpret_cast<char*>(&count), sizeof(count));
    errif(n != sizeof(count), "Failed to read from eventfd");
}

void EventLoop::wakeup()
{
    uint64_t count = 1;
    int n = wakeupFd_.write(reinterpret_cast<char*>(&count), sizeof(count));
    errif(n != sizeof(count), "Failed to write to eventfd");
}

bool EventLoop::isLoopThread() const
{
    return threadId_ == std::this_thread::get_id();
}

TimerId EventLoop::runAt(const Task& task, const TimeSpec& when)
{
    return timerQueue_.addTimer(task, when, 0.0);
}

TimerId EventLoop::runAfter(const Task& task, double delay)
{
    TimeSpec now;
    TimeSpec::getNow(now);
    return timerQueue_.addTimer(task, now + delay, 0.0);
}

TimerId EventLoop::runEvery(const Task& task, double interval)
{
    TimeSpec now;
    TimeSpec::getNow(now);
    return timerQueue_.addTimer(task, now + interval, interval);
}

void EventLoop::removeTimer(TimerId& timerId)
{
    timerQueue_.removeTimer(timerId);
}