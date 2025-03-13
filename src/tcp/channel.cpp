#include "channel.h"
#include "eventLoop.h"
#include <cstdint>
#include <sys/epoll.h>
#include <unistd.h>
Channel::Channel(int fd, const EventLoop* loop) : fd(fd), ep(loop), events(0), revents(0), inEpoll(false)
{
}

Channel::~Channel()
{
}

void Channel::enableReading()
{
    events = EPOLLIN | EPOLLET;
    ep->updateChannel(this);
}

int Channel::getFd() const
{
    return fd;
}
uint32_t Channel::getEvents() const
{
    return events;
}

uint32_t Channel::getRevents() const
{
    return revents;
}
bool Channel::getInEpoll() const
{
    return inEpoll;
}
void Channel::setInEpoll()
{
    inEpoll = true;
}

void Channel::setRevents(uint32_t revents)
{
    this->revents = revents;
}

void Channel::handleEvent() const
{
    cb();
}

void Channel::setCallback(std::function<void()> cb)
{
    this->cb = cb;
}