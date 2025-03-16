#include "eventLoop.h"
#include "channel.h"
#include "epoller.h"
#include "util.h"
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <vector>
EventLoop::EventLoop()
    : wakeupFd_(createEventfd()), wakeupChannel_(new Channel(wakeupFd_)), poller_(new Epoller()), tasks_()
{
    wakeupChannel_->setReadCb(std::bind(&EventLoop::handleRead, this));
    wakeupChannel_->enableRead();
    this->addChannel(wakeupChannel_.get());
}

EventLoop::~EventLoop() = default;
void EventLoop::loop()
{
    while (true)
    {
        std::vector<Channel*> activeChannels;
        std::queue<std::function<void()>> tempTasks;
        poller_->poll(activeChannels, -1);
        // 判断是否需要唤醒？
        needToWakeup_ = false;
        for (auto& channel : activeChannels)
        {
            channel->handleEvent();
        }

        needToWakeup_ = true;
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
void EventLoop::addChannel(Channel* channel) const
{
    poller_->addChannel(channel);
}
void EventLoop::updateChannel(Channel* channel) const
{
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel) const
{
    poller_->removeChannel(channel);
}

void EventLoop::runInLoop(const std::function<void()>& task)
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        tasks_.push(task);
    }
    if (needToWakeup_)
        wakeup();
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t n = ::read(wakeupFd_, &one, sizeof one);
    errif(n != sizeof one, "Failed to read from eventfd");
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = ::write(wakeupFd_, &one, sizeof one);
    errif(n != sizeof one, "Failed to write to eventfd");
}

int EventLoop::createEventfd()

{
    int evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    errif(evtfd < 0, "Failed to create eventfd");
    return evtfd;
}
