#include "iocontext.h"
#include "connection.h"
#include "epoller.h"
#include "ioobject.h"
#include "log.h"
#include "poller.h"
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
    enableRead(&waker);
}

IoContext::~IoContext() = default;
void IoContext::addAcceptor(InetAddress listenAddr)
{
    runInThread([this, listenAddr]() {
        auto acceptor = std::make_unique<Acceptor>(this, listenAddr);
        auto id = acceptor->id();
        this->ioObjects_[id] = std::move(acceptor);
        enableRead(id);
    });
}

void IoContext::addConnection(int clientSocket, InetAddress clientAddr, Acceptor* acceptor)
{
    runInThread([this, clientSocket, clientAddr, acceptor]() {
        auto connection = std::make_unique<Connection>(clientSocket, clientAddr, this, acceptor);
        ioObjects_[connection->id()] = std::move(connection);
        // 默认自动可读
        updateIoObject(connection.get(), Type::kReadable);
    });
}

void IoContext::updateIoObject(IoObject* object, Type type)
{
    auto it = ioObjects_.find(object->id());
    // 一定在？
    assert(it != ioObjects_.end());
    if (type == Type::kReadable)
    {
        re
    }
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
                static_cast<IoObject*>(ioObject)->onRead();
            if (activeType == Poller::Type::kWriteable || activeType == Poller::Type::kBoth)
                static_cast<IoObject*>(ioObject)->onWrite();
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

void IoContext::remove(IoObject* object)
{
    runInThread([this, object]() {
        poller_->update(object->fd(), object, Poller::Type::kStopped);
        ioObjects_.erase(object->id());
    });
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
