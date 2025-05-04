#include "epoller.h"
#include "channel.h"
#include <cassert>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>
namespace tcp
{
Epoller::Epoller() : epfd_(epoll_create1(EPOLL_CLOEXEC))
{
    assert(epfd_ >= 0);
}

Epoller::~Epoller()
{
    assert(epfd_ >= 0);
    close(epfd_);
}

std::vector<Channel*> Epoller::poll(int timeout)
{
    auto eventsNum = epoll_wait(epfd_, events_, kEventSize, timeout);
    assert(eventsNum >= 0);
    std::vector<Channel*> activeObjs(eventsNum);

    for (auto i = 0; i < eventsNum; ++i)
    {
        auto obj = static_cast<Channel*>(events_[i].data.ptr);
        auto event = events_[i].events;
        auto type = Channel::Type::kNone;
        if (event & EPOLLIN && event & EPOLLOUT)
        {
            type = Channel::Type::kBoth;
        }
        else if (event & EPOLLIN)
        {
            type = Channel::Type::kReadable;
        }
        else
            type = Channel::Type::kWriteable;
        obj->setExpiredType(type);
        activeObjs[i] = obj;
    }
    return activeObjs;
}

void Epoller::update(Channel* channel)
{
    assert(channel != nullptr);
    auto fd = channel->fd();
    assert(fd >= 0);
    auto it = channels_.find(fd);
    auto type = channel->type();
    if (type != Channel::kNone)
    {
        epoll_event ev;
        ev.events = 0;
        ev.data.ptr = channel;
        if (type == Channel::kReadable || type == Channel::kBoth)
        {
            ev.events |= EPOLLIN;
        }
        if (type == Channel::kWriteable || type == Channel::kBoth)
        {
            ev.events |= EPOLLOUT;
        }
        if (it == channels_.end())
        {
            epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev);
            channels_[fd] = channel;
        }
        else
            epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev);
    }
    else if (it != channels_.end())
    {
        epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, nullptr);
        channels_.erase(it);
    }
}
} // namespace tcp
