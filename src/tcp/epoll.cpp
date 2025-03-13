
#include "epoll.h"
#include "channel.h"
#include "util.h"
#include <sys/epoll.h>
#include <unistd.h>

Epoll::Epoll() : epfd(epoll_create(MAX_EVENTS)), events(new epoll_event[MAX_EVENTS])
{
    errif(epfd == -1, "epoll create error");
}

Epoll::~Epoll()
{
    if (epfd != -1)
    {
        close(epfd);
        epfd = -1;
    }
    delete[] events;
}

// 更新Channel的状态
void Epoll::updateChannel(Channel* channel) const
{
    if (channel)
    {
        int fd = channel->getFd();
        uint32_t events = channel->getEvents();
        epoll_event ev;
        ev.data.ptr = channel;
        ev.events = events;
        if (channel->getInEpoll())
            errif(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify event error");
        else
        {
            errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add event error");
            channel->setInEpoll();
        }
    }
}

// 获取事件
std::vector<Channel*> Epoll::poll(int timeout) const
{
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll wait error");
    std::vector<Channel*> activeChannels(nfds);
    for (int i = 0; i < nfds; ++i)
    {
        activeChannels[i] = reinterpret_cast<Channel*>(events[i].data.ptr);
        activeChannels[i]->setRevents(events[i].events);
    }
    return activeChannels;
}