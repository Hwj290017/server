#include "threadPool.h"

ThreadPool::ThreadPool(int threadNum) : threadNum(threadNum), stop(false)
{
    for (int i = 0; i < threadNum; ++i)
    {
        threads.emplace_back([this]() {
            while (true)
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [this]() { return stop || !tasks.empty(); });
                    if (stop && tasks.empty())
                        return;
                    task = tasks.front();
                    tasks.pop();
                }
                task();
            }
        });
    }
}

// 添加任务
void ThreadPool::add(std::function<void()> task)
{
    std::unique_lock<std::mutex> lock(mtx);
    if (stop)
        throw std::runtime_error("ThreadPool already stop, can't add task any more");
    tasks.push(task);
    cv.notify_one();
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;
    }
    cv.notify_all();
    for (std::thread& th : threads)
    {
        if (th.joinable())
            th.join();
    }
}