#pragma once
#include "channel.h"
#include "tcp/baseobject.h"
namespace tcp
{
template <typename T> struct BaseImpl
{
    enum State
    {
        kInit,
        kStarted,
        kPaused,
        kStopped
    };
    BaseImpl(int fd, std::size_t id, IoContext* ioContext, typename BaseObject<T>::StartTask&& startTask,
             typename BaseObject<T>::StopTask&& stopTask, typename BaseObject<T>::ReleaseTask&& releaseTask)
        : fd_(fd), ioContext_(ioContext), channel_(fd), id_(id), startTask_(std::move(startTask)),
          stopTask_(std::move(stopTask)), releaseTask_(std::move(releaseTask)), state_(kInit)
    {
    }
    int fd_;
    IoContext* ioContext_;
    Channel channel_;
    std::size_t id_;
    typename BaseObject<T>::StartTask startTask_;
    typename BaseObject<T>::StopTask stopTask_;
    typename BaseObject<T>::ReleaseTask releaseTask_;
    std::any context_;
    State state_;
};

} // namespace tcp