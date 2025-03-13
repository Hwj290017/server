
#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool
{
  private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    int threadNum;
    bool stop;

  public:
    ThreadPool(int size = std::thread::hardware_concurrency()); // 默认size最好设置为std::thread::hardware_concurrency()
    ~ThreadPool();
    void add(std::function<void()>);
};

#endif