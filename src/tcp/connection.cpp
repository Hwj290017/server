#include "tcp/connection.h"
#include "baseobjectimpl.h"
#include "buffer.h"
#include "iocontext.h"
#include "tcp/inetaddress.h"
#include "util/log.h"
#include <cassert>
#include <cerrno>
#include <cstddef>
#include <fcntl.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>
namespace tcp
{
struct Connection::Impl
{
    enum State
    {
        kConnected,
        kDisconnectd
    };
    InetAddress peerAddr_;
    MessageTask messageTask_;
    Buffer readBuffer_;
    Buffer writeBuffer_;
    State state_;

    Impl(const InetAddress& peerAddr, const MessageTask& messageTask)
        : peerAddr_(peerAddr), messageTask_(messageTask), state_(kDisconnectd)
    {
    }
};
Connection::Connection(int clientfd, IoContext* ioContext, std::size_t id, const BaseTasks& baseTasks,
                       const ReleaseTask& releaseTask, const InetAddress& peerAddr, const MessageTask& messageTask)
    : BaseObject(clientfd, ioContext, id, baseTasks, releaseTask), impl_(new Impl(peerAddr, messageTask))
{
    baseImpl_->channel_.setReadTask([this, readBuffer = Buffer(), task = messageTask]() mutable {
        Logger::logger << ("TcpConnection handleRead: " + std::to_string(baseImpl_->id_));

        if (readBuffer.readSocket(baseImpl_->fd_))
        {
            if (task)
                task(this, readBuffer.begin(), readBuffer.size());
        }
        else
        {
            stop();
        }
    });
    baseImpl_->channel_.setReadTask([this, writeBuffer = Buffer()]() mutable {
        if (writeBuffer.writeSocket(baseImpl_->fd_, std::string()))
        {
            if (writeBuffer.size() > 0)
            {
                ioContext_->updateChannel(&baseImpl_->channel_, Poller::Type::kBoth);
            }
        }
        else
        {
            stop();
        }
    });
}

Connection::~Connection() = default;

void Connection::send(const std::string& data)
{
    if (data.length() > 0)
    {
        if (impl_->writeBuffer_.writeSocket(baseImpl_->fd_, data))
        {
            if (impl_->writeBuffer_.size() > 0)
            {
                baseImpl_->ioContext_->updateChannel(&baseImpl_->channel_, Poller::Type::kBoth);
            }
        }
        else
        {
            stop();
        }
    }
}

void Connection::onWrite()
{
    // Logger::logger << ("TcpConnection handleWrite: " + id() + "\nWriteNum: " + std::to_string(writeNum));

    if (writeBuffer_.writeSocket(fd_, std::string()))
    {
        if (writeBuffer_.size() > 0)
        {
            ioContext_->updateObject(this, Poller::Type::kBoth);
        }
    }
    else
    {
        stop();
    }
}

void Connection::onRead()
{

    Logger::logger << ("TcpConnection handleRead: " + std::to_string(id_));

    if (readBuffer_.readSocket(fd_))
    {
        if (afterReadTask_)
            afterReadTask_(ConnectionId(id_), readBuffer_.begin(), readBuffer_.size());
    }
    else
    {
        stop();
    }
}

void Connection::setConnectTask(ConnectionId::ConnectTask&& task)
{
    connectTask_ = std::move(task);
}

void Connection::setDisconnectTask(ConnectionId::DisconnectTask&& task)
{
    disconnectTask_ = std::move(task);
}

void Connection::setAfterReadTask(ConnectionId::AfterReadTask&& task)
{
    afterReadTask_ = std::move(task);
}
} // namespace tcp
