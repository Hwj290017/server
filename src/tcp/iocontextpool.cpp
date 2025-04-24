#include "iocontext.h"
#include "eventLoop.h"
#include <functional>
namespace tcp
{
IoContext::IoContext() : loop_(nullptr)
{
}

// 析构函数，用于销毁IoContext对象
IoContext::~IoContext()
{
    // 如果线程是可连接的，则连接线程
    if (thread_.joinable())
        thread_.join();
}

// 创建线程并返回loop
EventLoop* IoContext::start()
{
    thread_ = std::thread(std::bind(&IoContext::threadFunc, this));
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while (!loop_)
            cv_.wait(lock);
    }
    return loop_;
}
// 线程函数
void IoContext::threadFunc()
{
    {
        std::unique_lock<std::mutex> lock(mutex_);
        loop_ = &loop;
        cv_.notify_one();
    }
    loop.loop();
    {
        std::unique_lock<std::mutex> lock(mutex_);
        loop_ = nullptr;
    }
}
} // namespace tcp
