#include "channel.h"
#include "eventLoop.h"
#include <cstdint>
#include <functional>
#include <sys/epoll.h>
#include <unistd.h>
Channel::Channel(int fd, const EventLoop* loop) : fd_(fd), loop_(loop), event_(0), revent_(0), inEpoll_(false)
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

bool Channel::isInEpoll() const
{
    return inEpoll_;
}

void Channel::setInEpoll()
{
    inEpoll_ = true;
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
    event_ = EPOLLIN | EPOLLET;
    loop_->updateChannel(this);
}

void Channel::enableWrite()
{
    event_ = EPOLLOUT | EPOLLET;
    loop_->updateChannel(this);
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

void Channel::close()
{
    loop_->closeChannel(this);
}