
#pragma once

#include "acceptor.h"
#include "inetAddress.h"
#include "ioobject.h"
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
#define MAX_EVENTS 1024
class Poller;

class IoContext
{
    using Task = std::function<void()>;

  public:
    IoContext();
    ~IoContext();
    void start();

    // void stop()
    // {
    //     runInThread([this]() { stopInThread(); });
    // }
    void addAcceptor(InetAddress listenAddr);
    void addConnection(int clientSocket, InetAddress clientAddr, Acceptor* acceptor);
    void remove(size_t id);
    void enableRead(size_t id);
    void disableRead(size_t id);
    void enableWrite(size_t id);
    void disableWrite(size_t id);

    void addTask(Task&& task);
    template <typename T> void runInThread(T&& task, double delay = 0.0, double interval = 0.0)
    {
        if (isOwnThread())
            task();
        else
            queueInThread(std::forward<T>(task));
    }

    template <typename T> void queueInThread(T&& task)
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
    class Waker : public IoObject
    {
      public:
        Waker(IoContext* context);
        void onRead() override;
        void wakeup();
    };
    void threadFunc();
    void stopInThread();
    std::unordered_map<std::size_t, std::unique_ptr<IoObject>> ioObjects_;
    std::unique_ptr<Poller> poller_;
    std::vector<Task> tasks_;
    Waker waker;
    std::mutex mutex_;
    State state_;
    std::thread::id threadId_;
};
} // namespace tcp
