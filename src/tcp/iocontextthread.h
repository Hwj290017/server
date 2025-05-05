#pragma once

#include "iocontext.h"
#include <thread>
namespace tcp
{

class IoContextThread
{
  public:
    IoContextThread() = default;
    ~IoContextThread()
    {
        ioContext_.stop();
        if (thread_.joinable())
            thread_.join();
    }
    void start()
    {
        thread_ = std::thread([ioContext = &ioContext_]() { ioContext->start(); });
    }
    void stop()
    {
        ioContext_.stop();
    }
    std::thread::id getThreadId() const
    {
        return thread_.get_id();
    }
    IoContext* getIoContext()
    {
        return &ioContext_;
    }

  private:
    IoContext ioContext_;
    std::thread thread_;
};

} // namespace tcp