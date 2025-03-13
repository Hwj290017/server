#ifndef LOOPTHREADPOOL_H
#define LOOPTHREADPOOL_H

#include "loopThread.h"
#include <thread>
#include <vector>
class EventLoop;
class LoopThreadPool
{
  public:
    LoopThreadPool(const EventLoop* mainLoop, int threadNum = std::thread::hardware_concurrency());
    LoopThreadPool(const LoopThreadPool&) = delete;
    LoopThreadPool(LoopThreadPool&&) = delete;
    ~LoopThreadPool();
    void start();
    void stop();
    EventLoop* nextLoop();

  private:
    const EventLoop* mainLoop_;
    std::vector<LoopThread> loopThreads_;
    std::vector<EventLoop*> loops_;
    int threadNum_;
    int next_;
};
#endif // LOOPTHREADPOOL_H