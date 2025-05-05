#pragma once
#include "baseimpl.h"
#include "buffer.h"
#include "tcp/connection.h"
namespace tcp
{
// 特化ConnectionImpl
template <> struct Impl<Connection> : public BaseImpl<Connection>
{
    Impl(int fd, IoContext* ioContext, std::size_t id, const InetAddress& peerAddr_, const Connection::Tasks& tasks,
         const Connection::ReleaseTask& releaseTaskTasks)
        : BaseImpl<Connection>(fd, id, ioContext, tasks.startTask, tasks.stopTask, releaseTaskTasks),
          peerAddr_(peerAddr_), messageTask_(tasks.messageTask)
    {
    }
    InetAddress peerAddr_;
    Connection::MessageTask messageTask_;
    Buffer readBuffer_;
    Buffer writeBuffer_;
};
} // namespace tcp