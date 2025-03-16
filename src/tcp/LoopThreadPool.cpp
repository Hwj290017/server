#include "LoopThreadPool.h"
#include "EventLoop.h"
#include "loopThread.h"
#include <memory>

LoopThreadPool::LoopThreadPool(int threadNum) : threadNum_(threadNum), next_(0)
{
}

LoopThreadPool::~LoopThreadPool()
{
}

// 创建子线程
void LoopThreadPool::start()
{
    loopThreads_.reserve(threadNum_);
    for (int i = 0; i < threadNum_; ++i)
    {
        loopThreads_.emplace_back(std::make_unique<LoopThread>());
        loops_.emplace_back(loopThreads_.back()->startLoop());
    }
}

// 返回一个EventLoop
EventLoop* LoopThreadPool::nextLoop()
{
    EventLoop* loop = loops_[next_];
    next_ = (next_ + 1) % threadNum_;
    return loop;
}