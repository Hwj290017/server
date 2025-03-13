#include "LoopThreadPool.h"
#include "EventLoop.h"
#include "loopThread.h"
#include <iostream>

LoopThreadPool::LoopThreadPool(const EventLoop* mainLoop, int threadNum)
    : mainLoop_(mainLoop), threadNum_(threadNum), next_(0)
{
    std::cout << "LoopThreadPool created" << std::endl;
}

LoopThreadPool::~LoopThreadPool()
{
}

// 创建子线程
void LoopThreadPool::start()
{
    for (int i = 0; i < threadNum_; ++i)
    {
        loopThreads_.emplace_back();
        loops_.emplace_back(loopThreads_.back().startLoop());
    }
}

// 返回一个EventLoop
EventLoop* LoopThreadPool::nextLoop()
{
    EventLoop* loop = loops_[next_];
    next_ = (next_ + 1) % threadNum_;
    return loop;
}