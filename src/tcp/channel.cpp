#include "channel.h"
#include "eventLoop.h"
#include <cstdint>
#include <functional>
#include <sys/epoll.h>
#include <unistd.h>
Channel::Channel(int fd) : fd_(fd), event_(0), revent_(0)
{
}

Channel::~Channel()
{
}

int Channel::getFd() const
{
    return fd_;
}

uint32_t Channel::getEvent() const
{
    return event_;
}

uint32_t Channel::getRevent() const
{
    return revent_;
}

void Channel::setRevent(uint32_t revent)
{
    this->revent_ = revent;
}

void Channel::setReadCb(std::function<void()> cb)
{
    readCb_ = cb;
}

void Channel::setWriteCb(std::function<void()> cb)
{
    writeCb_ = cb;
}

void Channel::enableRead()
{
    event_ |= EPOLLIN;
}

void Channel::enableWrite()
{
    event_ |= EPOLLOUT;
}

void Channel::enableEt()
{
    event_ |= EPOLLET;
}
void Channel::handleEvent() const
{
    if ((revent_ | EPOLLIN) && readCb_)
    {
        readCb_();
    }

    if ((revent_ | EPOLLOUT) && writeCb_)
    {
        writeCb_();
    }
}
