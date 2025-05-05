#pragma once

#include "baseimpl.h"
#include "buffer.h"
#include "tcp/connector.h"

namespace tcp
{

// 特化ConnectorImpl
template <> struct Impl<Connector> : public BaseImpl<Connector>
{
    Impl(int fd, IoContext* ioContext, std::size_t id, const InetAddress& serverAddr, const Connector::Tasks& tasks,
         const Connector::ReleaseTask& releaseTaskTasks)
        : BaseImpl<Connector>(fd, id, ioContext, tasks.startTask, tasks.stopTask, releaseTaskTasks),
          serverAddr_(serverAddr), messageTask_(tasks.messageTask)
    {
    }
    InetAddress serverAddr_;
    Connector::MessageTask messageTask_;
    Buffer readBuffer_;
    Buffer writeBuffer_;
};

} // namespace tcp