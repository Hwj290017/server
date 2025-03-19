#include "epoller.h"
#include "EventLoop.h"
#include "channel.h"
#include "util.h"
#include <cassert>
#include <cstdint>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>
Epoller::Epoller() : epfd_(createEpollFd_())
{
}

Epoller::~Epoller()
{
    assert(epfd_ >= 0);
    close(epfd_);
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

void Epoller::updateChannel(Channel* channel)
{
    if (channel)
    {
        int fd = channel->getFd();
        uint32_t event = channel->getEvent();
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
        int fd = channel->getFd();
        return channels_.find(fd) != channels_.end();
    }
    return false;
}

int Epoller::createEpollFd_()
{
    int epfd = epoll_create1(EPOLL_CLOEXEC);
    errif(epfd < 0, "epoll_create1 error");
    return epfd;
}