#ifndef CHANNEL_H
#define CHANNEL_H

#include <cstdint>
#include <functional>
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

    void quit()
    {
        isQuit_ = true;
    }
    void start()
    {
        isQuit_ = false;
    }
    RWAbleFd* fd() const
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