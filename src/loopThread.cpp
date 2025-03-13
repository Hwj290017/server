#include "loopThread.h"
#include "EventLoop.h"
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

LoopThread::LoopThread() : loop_(nullptr)
{
    std::cout << "loopThread created" << std::endl;
}
LoopThread::LoopThread(LoopThread&& other) noexcept : loop_(other.loop_), thread_(std::move(other.thread_))
{
    std::cout << "loopThread moved" << std::endl;
}
LoopThread::~LoopThread()
{
    if (thread_.joinable())
        thread_.join();
}

// 创建线程并返回loop
EventLoop* LoopThread::startLoop()
{
    thread_ = std::thread(std::bind(&LoopThread::threadFunc, this));
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while (!loop_)
            cv_.wait(lock);
    }
    return loop_;
}
// 线程函数
void LoopThread::threadFunc()
{
    EventLoop loop;
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