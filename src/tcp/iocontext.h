
#pragma once
#include "channel.h"
#include "poller.h"
#include <cassert>
#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <sys/eventfd.h>
#include <thread>
#include <vector>
namespace tcp
{
class SharedObject;
class IoContext
{
    using Task = std::function<void()>;

  public:
    IoContext();
    ~IoContext();
    void start();
    void stop();
    // 以下接口由调用方保证保证在同一个线程中调用
    void updateChannel(Channel* channel);

    template <typename T> void runTask(T&& task, double delay = 0.0, double interval = 0.0)
    {
        if (inOwnThread())
            task();
        else
            queueTask(std::forward<T>(task));
    }

    template <typename T> void queueTask(T&& task)
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            tasks_.emplace_back(std::forward<T>(task));
        }
        // 无论是不是当前线程，只要不是在处理事件都唤醒
        if (state_ != HandlingEvents)
            waker.wakeup();
    }

    bool inOwnThread() const;

  private:
    enum State
    {
        kStopped,
        HandlingEvents,
        CallingTasks,
        Waiting
    };
    class Waker : public Channel
    {
      public:
        Waker();
        void onRead();
        void wakeup();
    };

    std::unique_ptr<Poller> poller_;
    std::vector<Task> tasks_;
    Waker waker;
    std::mutex mutex_;
    State state_;
    std::thread::id threadId_;
    std::size_t id_;
};
} // namespace tcp
