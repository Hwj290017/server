#include "LoopThreadPool.h"
#include "EventLoop.h"
#include "log.h"
#include "loopThread.h"

LoopThreadPool::LoopThreadPool(const EventLoop* mainLoop, int threadNum)
    : mainLoop_(mainLoop), threadNum_(threadNum), next_(0)
{
    logger << "LoopThreadPool created\n";
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