#pragma once
#include "channel.h"
#include "tcp/baseobject.h"
namespace tcp
{
template <typename T> struct BaseImpl
{
    BaseImpl(int fd, std::size_t id, IoContext* ioContext, const typename BaseObject<T>::BaseTasks& baseTasks)
        : fd_(fd), ioContext_(ioContext), channel_(fd), id_(id), baseTasks_(baseTasks)
    {
    }
    int fd_;
    IoContext* ioContext_;
    Channel channel_;
    std::size_t id_;
    typename BaseObject<T>::BaseTasks baseTasks_;
    std::any context_;
};

} // namespace tcp