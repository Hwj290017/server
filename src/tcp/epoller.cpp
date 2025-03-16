#include "epoller.h"
#include "EventLoop.h"
#include "channel.h"
#include "util.h"
#include <cstdint>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>
Epoller::Epoller() : epfd_(epoll_create(MAX_EVENTS)), events_(new epoll_event[MAX_EVENTS])
{
    errif(epfd_ < 0, "epoll_create error");
}

Epoller::~Epoller()
{
    close(epfd_);
    delete[] events_;
}

void Epoller::poll(std::vector<Channel*>& activeChannels, int timeout)
{
    int eventsNum = epoll_wait(epfd_, events_, MAX_EVENTS, timeout);
    errif(eventsNum < 0, "epoll_wait error");
    activeChannels.reserve(eventsNum);
    for (int i = 0; i < eventsNum; ++i)
    {
        Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
        channel->setRevent(events_[i].events);
        activeChannels.emplace_back(channel);
    }
}

void Epoller::addChannel(Channel* channel)
{
    if (channel)
    {
        int fd = channel->getFd();
        uint32_t event = channel->getEvent();
        errif(channels_.find(fd) != channels_.end(), "channel already added");

        epoll_event ev;
        ev.events = event;
        ev.data.ptr = channel;
        epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev);
        channels_[fd] = channel;
    }
}
void Epoller::updateChannel(Channel* channel)
{
    if (channel)
    {
        int fd = channel->getFd();
        uint32_t event = channel->getEvent();
        errif(channels_.find(fd) == channels_.end(), "channel not added");

        epoll_event ev;
        ev.events = event;
        ev.data.ptr = channel;
        epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev);
    }
}

void Epoller::removeChannel(Channel* channel)
{
    if (channel)
    {
        int fd = channel->getFd();
        errif(channels_.find(fd) == channels_.end(), "channel not added");

        epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, nullptr);
        channels_.erase(fd);
    }
}

bool Epoller::hasChannel(Channel* channel)
{
    if (channel)
    {
        int fd = channel->getFd();
        return channels_.find(fd) != channels_.end();
    }
    return false;
}