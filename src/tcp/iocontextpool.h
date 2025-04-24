#pragma once
#include "eventLoop.h"
#include <condition_variable>
#include <mutex>
#include <thread>
namespace tcp
{
class IoContext
{
  public:
    IoContext();
    ~IoContext();
    void start();

  private:
    void threadFunc();

    std::thread thread_;
    // 在线程函数中创建的栈对象自动释放
    EventLoop loop_;
    // 主线程和子线程访问loop同步
    std::mutex mutex_;
};
} // namespace tcp
