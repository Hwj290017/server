#include "EventLoop.h"
#include "channel.h"
#include "util.h"
#include <sys/epoll.h>
#include <unistd.h>

EventLoop::EventLoop() : epfd(-1), quit(false), events(nullptr)
{
    epfd = epoll_create(MAX_EVENTS);
    events = new epoll_event[MAX_EVENTS];
    errif(epfd == -1, "epoll create error");
}
EventLoop::~EventLoop()
{
    quit = true;
    if (epfd >= 0)
    {
        close(epfd);
        epfd = -1;
    }
}
void EventLoop::loop() const
{
    while (!quit)
    {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        errif(nfds == -1, "epoll wait error");
        for (int i = 0; i < nfds; i++)
        {
            Channel* channel = reinterpret_cast<Channel*>(events[i].data.ptr);
            channel->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel* channel) const
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