#pragma once
#include "baseimpl.h"
#include "tcp/acceptor.h"
namespace tcp
{

// 特化AcceptorImpl
template <> struct Impl<Acceptor> : public BaseImpl<Acceptor>
{
    Impl(int fd, IoContext* ioContext, std::size_t id, const InetAddress& listenAddr, const Acceptor::Tasks& Tasks,
         const Acceptor::ReleaseTask& releaseTask)
        : BaseImpl<Acceptor>(fd, id, ioContext, Tasks.startTask, Tasks.stopTask, releaseTask), listenAddr_(listenAddr),
          acceptTask_(Tasks.acceptTask) {};
    InetAddress listenAddr_;
    Acceptor::AcceptTask acceptTask_;
};
} // namespace tcp