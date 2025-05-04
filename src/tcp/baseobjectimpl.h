#pragma once
#include "baseimpl.h"
#include "buffer.h"
#include "channel.h"
#include "iocontext.h"
#include "tcp/acceptor.h"
#include "tcp/baseobject.h"
#include "tcp/connection.h"
#include "tcp/connector.h"
#include "tcp/tempptr.h"
#include <any>
#include <cstddef>
#include <type_traits>
namespace tcp
{

// 特化AcceptorImpl
template <> struct Impl<Acceptor> : public BaseImpl<Acceptor>
{
    Impl(int fd, IoContext* ioContext, std::size_t id, Acceptor::BaseTasks&& baseTasks,
         Acceptor::ReleaseTask&& releaseTask, InetAddress&& listenAddr_, const Acceptor::AcceptTask& acceptTask)
        : BaseImpl<Acceptor>(fd, id, ioContext, std::move(baseTasks), std::move(releaseTask)),
          listenAddr_(std::move(listenAddr_)), acceptTask_(acceptTask) {};
    InetAddress listenAddr_;
    Acceptor::AcceptTask acceptTask_;
};

// 特化ConnectionImpl
template <> struct Impl<Connection> : public BaseImpl<Connection>
{
    Impl(int fd, IoContext* ioContext, std::size_t id, Connection::BaseTasks&& baseTasks,
         Connection::ReleaseTask&& releaseTaskTasks, InetAddress&& peerAddr_,
         const Connection::MessageTask& messageTask)
        : BaseImpl<Connection>(fd, id, ioContext, std::move(baseTasks), std::move(releaseTaskTasks)),
          peerAddr_(std::move(peerAddr_)), messageTask_(messageTask)
    {
    }
    InetAddress peerAddr_;
    Connection::MessageTask messageTask_;
    Buffer readBuffer_;
    Buffer writeBuffer_;
};

// 特化ConnectorImpl
template <> struct Impl<Connector> : public BaseImpl<Connector>
{
    Impl(int fd, IoContext* ioContext, std::size_t id, Connector::BaseTasks&& baseTasks,
         Connection::ReleaseTask&& releaseTaskTasks, InetAddress&& serverAddr,
         const Connector::MessageTask& massageTask)
        : BaseImpl<Connector>(fd, id, ioContext, std::move(baseTasks), std::move(releaseTaskTasks)),
          serverAddr_(std::move(serverAddr)), messageTask_(massageTask)
    {
    }
    InetAddress serverAddr_;
    Connector::MessageTask messageTask_;
    Buffer readBuffer_;
    Buffer writeBuffer_;
};
// 平凡定义
template <typename T> struct Impl : public BaseImpl<T>
{
};

template <typename T> void BaseObject<T>::start(double delay)
{
    static_assert(std::is_base_of_v<BaseObject<T>, T>);
    static_assert(std::is_base_of_v<BaseImpl<T>, Impl<T>>);
    if (delay <= 0.0)
    {
        if (impl_->baseTasks_.startTask)
        {
            impl_->baseTasks_.startTask(TempPtr<T>(static_cast<T*>(this)));
        }
        impl_->ioContext_->updateChannel(&impl_->channel_);
    }
}

template <typename T> void BaseObject<T>::stop(double delay)
{
    static_assert(std::is_base_of_v<BaseObject<T>, T>);
    if (delay <= 0.0)
    {
        if (impl_->baseTasks_.stopTask)
        {
            impl_->baseTasks_.stopTask(TempPtr<T>(static_cast<T*>(this)));
        }
        impl_->ioContext_->updateChannel(&impl_->channel_);
        impl_->releaseTask_(impl_->id_);
    }
}
template <typename T> void BaseObject<T>::doTask(const Task& task, double delay, double interval)
{
    static_assert(std::is_base_of_v<BaseObject<T>, T>);
    impl_->ioContext_->runTask(task, delay, interval);
}

template <typename T> void BaseObject<T>::setContext(std::any context)
{
    static_assert(std::is_base_of_v<BaseObject<T>, T>);
    impl_->context_ = std::move(context);
}

template <typename T> std::any& BaseObject<T>::getContext()
{
    static_assert(std::is_base_of_v<BaseObject<T>, T>);
    return impl_->context_;
}
template <typename T> std::size_t BaseObject<T>::id() const
{
    static_assert(std::is_base_of_v<BaseObject<T>, T>);
    return impl_->id_;
}

template class BaseObject<Acceptor>;
template class BaseObject<Connection>;
template class BaseObject<Connector>;
} // namespace tcp