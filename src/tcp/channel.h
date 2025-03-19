#ifndef CHANNEL_H
#define CHANNEL_H

#include <cstdint>
#include <functional>

class Channel
{
    typedef std::function<void()> Task;

  public:
    Channel(int fd) : fd_(fd), quit_(false), event_(0), revent_(0)
    {
    }
    ~Channel();

    void enableRead();
    void enableWrite();
    void enableEt();
    void handleEvent() const;

    void quit()
    {
        quit_ = true;
    }
    void start()
    {
        quit_ = false;
    }
    int getFd() const
    {
        return fd_;
    }
    uint32_t getEvent() const
    {
        return event_;
    }
    uint32_t getRevent() const
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
    void setReadCb(Task&& cb)
    {
        readCb_ = std::move(cb);
    }
    void setWriteCb(const Task& cb)
    {
        writeCb_ = cb;
    }
    void setWriteCb(Task&& cb)
    {
        writeCb_ = std::move(cb);
    }
    bool isQuit()
    {
        return quit_;
    }

  private:
    int fd_;
    bool quit_;
    uint32_t event_;
    uint32_t revent_;
    Task readCb_;
    Task writeCb_;
};
#endif