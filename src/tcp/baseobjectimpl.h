#pragma once
#include "baseimpl.h"
#include "channel.h"
#include "iocontext.h"
#include "tcp/acceptor.h"
#include "tcp/baseobject.h"
#include "tcp/connector.h"
#include "tcp/tempptr.h"
#include <any>
#include <cstddef>
#include <type_traits>
namespace tcp
{
template <typename T> struct BaseObject<T>::Impl : public BaseImpl<T>
{
    Impl(int fd, IoContext* ioContext, std::size_t id, const BaseTasks& tasks)
        : BaseImpl<T>(fd, id, ioContext, tasks) {};
};

template <typename T> void BaseObject<T>::start(double delay)
{
    static_assert(std::is_base_of_v<BaseObject<T>, T>);
    static_assert(std::is_base_of_v<BaseImpl<T>, Impl>);
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