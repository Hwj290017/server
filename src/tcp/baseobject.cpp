
#include "tcp/baseobject.h"
#include "accptorimpl.h"
#include "baseimpl.h"
#include "channel.h"
#include "connectionimpl.h"
#include "connectorimpl.h"
#include "iocontext.h"
#include "tcp/acceptor.h"
#include "tcp/connection.h"
#include "tcp/connector.h"
#include "tcp/tempptr.h"
#include <any>
#include <cstddef>
#include <type_traits>
namespace tcp
{
// 平凡定义
template <typename T> struct Impl : public BaseImpl<T>
{
};

template <typename T> BaseObject<T>::BaseObject() = default;
template <typename T> BaseObject<T>::BaseObject(BaseObject&&) noexcept = default;
template <typename T> BaseObject<T>::~BaseObject() = default;
template <typename T> void BaseObject<T>::start(double delay)
{
    static_assert(std::is_base_of_v<BaseObject<T>, T>);
    static_assert(std::is_base_of_v<BaseImpl<T>, Impl<T>>);
    if (delay <= 0.0)
    {
        if (impl_->startTask_)
        {
            impl_->startTask_(TempPtr<T>(static_cast<T*>(this)));
        }
        impl_->channel_.setType(Channel::Type::kReadable);
        impl_->ioContext_->updateChannel(&impl_->channel_);
    }
}

template <typename T> void BaseObject<T>::stop(double delay)
{
    static_assert(std::is_base_of_v<BaseObject<T>, T>);
    if (delay <= 0.0)
    {
        if (impl_->stopTask_)
        {
            impl_->stopTask_(TempPtr<T>(static_cast<T*>(this)));
        }
        impl_->channel_.setType(Channel::Type::kNone);
        impl_->ioContext_->updateChannel(&impl_->channel_);
        impl_->releaseTask_(impl_->id_);
    }
}
template <typename T> void BaseObject<T>::doTask(const Task& task, double delay, double interval)
{
    static_assert(std::is_base_of_v<BaseObject<T>, T>);
    impl_->ioContext_->runTask([this, task = task]() { task(TempPtr<T>(static_cast<T*>(this))); }, delay, interval);
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