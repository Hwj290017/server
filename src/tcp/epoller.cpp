#include "epoller.h"
#include "EventLoop.h"
#include "channel.h"
#include "util.h"
#include <cassert>
#include <sys/epoll.h>
#include <unistd.h>

Epoller::Epoller() : epfd_(createEpollFd_())
{
}

Epoller::~Epoller()
{
    assert(epfd_ >= 0);
    close(epfd_);
}

std::vector<Channel*> Epoller::poll(int timeout)
{
    auto eventsNum = epoll_wait(epfd_, events_, MAX_EVENTS, timeout);
    errif(eventsNum < 0, "epoll_wait error");
    std::vector<Channel*> activeChannels(eventsNum);

    for (auto i = 0; i < eventsNum; ++i)
    {
        auto channel = static_cast<Channel*>(events_[i].data.ptr);
        channel->setRevent(events_[i].events);
        activeChannels[i] = channel;
    }
    return activeChannels;
}

void Epoller::updateChannel(Channel* channel)
{
    if (channel)
    {
        auto fd = channel->fd();
        auto event = channel->getEvent();
        epoll_event ev;
        ev.events = event;
        ev.data.ptr = channel;
        auto it = channels_.find(fd);
        if (channel->isQuit())
        {
            // 删除
            assert(it != channels_.end());
            epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, nullptr);
        }
        else
        {
            if (it == channels_.end())
            {
                // 添加
                channels_[fd] = channel;
                epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev);
            }
            else
            {
                // 修改
                epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev);
            }
        }
    }
}

bool Epoller::hasChannel(const Channel* channel)
{
    if (channel)
    {
        auto fd = channel->fd();
        return channels_.find(fd) != channels_.end();
    }
    return false;
}

int Epoller::createEpollFd_()
{
    auto epfd = epoll_create1(EPOLL_CLOEXEC);
    errif(epfd < 0, "epoll_create1 error");
    return epfd;
}