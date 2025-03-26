#ifndef CHANNEL_H
#define CHANNEL_H

#include "RWAbleFd.h"
#include <cstdint>
#include <functional>
#include <utility>
class RWAbleFd;
class Channel
{
    typedef std::function<void()> Task;

  public:
    Channel(RWAbleFd* fd) : fd_(fd), event_(0), revent_(0), isQuit_(false)
    {
    }
    ~Channel() = default;

    void enableRead();
    void enableWrite();
    void enableEt();
    void handleEvent() const;

    void close()
    {
        isQuit_ = true;
    }
    void start()
    {
        isQuit_ = false;
    }
    auto fd() const
    {
        return fd_->fd();
    }
    auto getEvent() const
    {
        return event_;
    }
    auto getRevent() const
    {
        return revent_;
    }
    void setRevent(uint32_t revent)
    {
        revent_ = revent;
    }
    void setReadCb(const Task& cb)
    {
        readCb_ = cb;
    }

    template <typename T> void setReadCb(T&& cb)
    {
        readCb_ = std::forward<T>(cb);
    }

    template <typename T> void setWriteCb(T&& cb)
    {
        writeCb_ = std::forward<T>(cb);
    }

    auto isQuit()
    {
        return isQuit_;
    }

  private:
    RWAbleFd* fd_;
    uint32_t event_;
    uint32_t revent_;
    Task readCb_;
    Task writeCb_;
    bool isQuit_;
};
#endif