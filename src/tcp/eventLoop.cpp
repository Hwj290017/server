#include "eventLoop.h"
#include "channel.h"
#include "epoller.h"
#include <functional>
#include <memory>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>
EventLoop::EventLoop() : poller_(new Epoller()), tasks_()
{
}

EventLoop::~EventLoop() = default;
void EventLoop::loop()
{
    while (true)
    {
        std::vector<Channel*> activeChannels;
        poller_->poll(activeChannels, -1);
        for (auto& channel : activeChannels)
        {
            channel->handleEvent();
        }

        while (!tasks_.empty())
        {
            std::function<void()>& task = tasks_.front();
            task();
            tasks_.pop();
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

void EventLoop::runInLoop(std::function<void()> task)
{
    tasks_.push(std::move(task));
}