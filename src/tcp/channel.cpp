#include "channel.h"
#include <cassert>
#include <sys/epoll.h>
#include <unistd.h>

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
