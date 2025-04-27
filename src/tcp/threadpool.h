#pragma once

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
namespace tcp
{

class ThreadPool
{
  public:
    using Task = std::function<void()>;
    static constexpr size_t kInitialThreadNum = 8;
    // 构造函数，创建指定数量的工作线程
    void addTask(Task task)
    {
        if (!isStopped_)
        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            tasks_.push(std::move(task));
            condition_.notify_one();
        }
    }

    void stop();
    static ThreadPool& Instance()
    {
        static ThreadPool instance;
        return instance;
    }

  private:
    ThreadPool(size_t threadNum = kInitialThreadNum) : isStopped_(false)
    {
        for (size_t i = 0; i < threadNum; ++i)
        {
            workers_.emplace_back([this] {
                while (true)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queueMutex_);
                        this->condition_.wait(lock, [this] { return this->isStopped_ || !this->tasks_.empty(); });

                        if (this->isStopped_ && this->tasks_.empty())
                            return;

                        task = std::move(this->tasks_.front());
                        this->tasks_.pop();
                    }
                    task();
                }
            });
        }
    }
    bool isStopped_;
    std::vector<std::thread> workers_;
    std::queue<Task> tasks_;
    std::mutex queueMutex_;
    std::condition_variable condition_;
};

} // namespace tcp