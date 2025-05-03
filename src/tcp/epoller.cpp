#include "epoller.h"
#include "channel.h"
#include <cassert>
#include <sys/epoll.h>
#include <unistd.h>
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

auto Epoller::poll(int timeout) -> std::vector<ActiveObj>
{
    auto eventsNum = epoll_wait(epfd_, events_, kEventSize, timeout);
    assert(eventsNum >= 0);
    std::vector<ActiveObj> activeObjs(eventsNum);

    for (auto i = 0; i < eventsNum; ++i)
    {
        auto obj = static_cast<Channel*>(events_[i].data.ptr);
        auto event = events_[i].events;
        auto type = Type::kNone;
        if (event & EPOLLIN && event & EPOLLOUT)
        {
            type = Type::kBoth;
        }
        else if (event & EPOLLIN)
        {
            type = Type::kReadable;
        }
        else
            type = Type::kWriteable;
        activeObjs[i] = {obj, type};
    }
    return activeObjs;
}

void Epoller::update(Channel* channel, Type type)
{
    assert(channel != nullptr);
    auto fd = channel->fd();
    assert(fd >= 0);
    auto it = attachedFds_.find(fd);
    if (type != Type::kNone)
    {
        epoll_event ev;
        ev.events = 0;
        ev.data.ptr = channel;
        if (type == Type::kReadable || type == Type::kBoth)
        {
            ev.events |= EPOLLIN;
        }
        if (type == Type::kWriteable || type == Type::kBoth)
        {
            ev.events |= EPOLLOUT;
        }
        if (it == attachedFds_.end())
        {
            epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev);
        }
        else
            epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev);
    }
    else if (it != attachedFds_.end())
    {
        epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, nullptr);
    }
}
} // namespace tcp
