#include "EventLoop.h"
#include "channel.h"
#include "epoll.h"
EventLoop::EventLoop() : ep(new Epoll()), quit(false)
{
}
EventLoop::~EventLoop()
{
    quit = true;
    delete ep;
}
void EventLoop::loop() const
{
    while (!quit)
    {
        std::vector<Channel*> activeChannels = ep->poll();
        for (auto channel : activeChannels)
        {
            channel->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel* channel) const
{
    ep->updateChannel(channel);
}