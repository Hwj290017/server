#include "iocontext.h"
#include "connection.h"
#include "epoller.h"
#include "object.h"
#include "poller.h"
#include "util/log.h"
#include <cassert>
#include <cstdint>
#include <string>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <vector>
namespace tcp
{
IoContext::IoContext() : poller_(new Epoller()), waker(this), tasks_(), state_(kStopped)
{
}

void IoContext::start()
{
    threadId_ = std::this_thread::get_id();
    // 循环执行
    while (state_ != kStopped)
    {
        // 创建临时任务队列
        std::vector<std::function<void()>> tempTasks;
        // 等待事件
        state_ = Waiting;
        auto activeObj = poller_->poll(-1);
        Logger::logger << ("activeIoObjects size: " + std::to_string(activeObj.size()));
        // 处理时间
        state_ = HandlingEvents;
        for (auto [ioObject, activeType] : activeObj)
        {
            if (activeType == Poller::Type::kReadable || activeType == Poller::Type::kBoth)
                static_cast<Object*>(ioObject)->onRead();
            if (activeType == Poller::Type::kWriteable || activeType == Poller::Type::kBoth)
                static_cast<Object*>(ioObject)->onWrite();
        }

        // 处理任务队列
        state_ = CallingTasks;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            tempTasks.swap(tasks_);
        }
        for (auto& task : tempTasks)
        {
            task();
        }
    }
}

bool IoContext::inOwnThread() const
{
    return threadId_ == std::this_thread::get_id();
}

void IoContext::updateObject(Object* object, Poller::Type type)
{
    poller_->update(object->fd(), object, type);
}

IoContext::Waker::Waker(IoContext* ioContext) : Object(eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK), ioContext)
{
    ioContext_->updateObject(this, Poller::Type::kReadable);
}
void IoContext::Waker::wakeup()
{
    uint64_t count = 1;
    int n = ::write(fd_, reinterpret_cast<char*>(&count), sizeof(count));
    assert(n == sizeof(count));
}

void IoContext::Waker::onRead()
{
    uint64_t count;
    int n = ::read(fd_, reinterpret_cast<char*>(&count), sizeof(count));
    assert(n == sizeof(count));
}

} // namespace tcp
