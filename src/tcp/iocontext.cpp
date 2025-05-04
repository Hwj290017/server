#include "iocontext.h"
#include "channel.h"
#include "epoller.h"
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
        auto activeChannels = poller_->poll(-1);
        Logger::logger << ("activeIoObjects size: " + std::to_string(activeChannels.size()));
        // 处理时间
        state_ = HandlingEvents;
        for (auto channel : activeChannels)
        {
            channel->onEvent();
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

void IoContext::updateChannel(Channel* channel)
{
    poller_->update(channel);
}

// waker初始化就启动
IoContext::Waker::Waker(IoContext* ioContext)
    : fd_(eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK)), channel_(fd_), ioContext_(ioContext)
{
    assert(fd_ >= 0);
    channel_.setType(Channel::kReadable);
    channel_.setReadTask([this] {
        uint64_t count;
        int n = ::read(fd_, reinterpret_cast<char*>(&count), sizeof(count));
        assert(n == sizeof(count));
    });
    ioContext_->updateChannel(&channel_);
}
void IoContext::Waker::wakeup()
{
    uint64_t count = 1;
    int n = ::write(fd_, reinterpret_cast<char*>(&count), sizeof(count));
    assert(n == sizeof(count));
}

} // namespace tcp
