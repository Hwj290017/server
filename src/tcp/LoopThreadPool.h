#ifndef LOOPTHREADPOOL_H
#define LOOPTHREADPOOL_H

#include "loopThread.h"
#include <memory>
#include <thread>
#include <vector>
class EventLoop;
class LoopThreadPool
{
  public:
    LoopThreadPool(int threadNum = std::thread::hardware_concurrency());
    LoopThreadPool(const LoopThreadPool&) = delete;
    LoopThreadPool(LoopThreadPool&&) = delete;
    ~LoopThreadPool();
    void start();
    void stop();
    EventLoop* nextLoop();

  private:
    std::vector<std::unique_ptr<LoopThread>> loopThreads_;
    std::vector<EventLoop*> loops_;
    int threadNum_;
    int next_;
};
#endif // LOOPTHREADPOOL_H