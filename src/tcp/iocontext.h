
#pragma once
#include "poller.h"
#include "sharedobject.h"
#include <cassert>
#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <sys/eventfd.h>
#include <thread>
#include <unordered_map>
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
    // 以下接口由调用方保证保证在同一个线程中调用
    void updateObject(SharedObject* object, Poller::Type type);
    // 以下接口线程安全
    std::size_t id();
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
    class Waker : public Object
    {
      public:
        Waker();
        void onRead() override;
        void wakeup();
    };

    std::unique_ptr<Poller> poller_;
    std::vector<Task> tasks_;
    Waker waker;
    std::mutex mutex_;
    State state_;
    std::thread::id threadId_;
    std::size_t id_;
    static std::atomic<std::size_t> nextId_;
};
} // namespace tcp
