#ifndef LOOPTHREAD_H
#define LOOPTHREAD_H

#include <condition_variable>
#include <mutex>
#include <thread>

class EventLoop;
class LoopThread
{
  public:
    LoopThread();
    ~LoopThread();
    EventLoop* startLoop();

  private:
    void threadFunc();

    std::thread thread_;
    // 在线程函数中创建的栈对象自动释放
    EventLoop* loop_;
    // 主线程和子线程访问loop同步
    std::mutex mutex_;
    std::condition_variable cv_;
};
#endif