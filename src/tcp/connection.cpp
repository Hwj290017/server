#include "tcp/connection.h"
#include "baseobjectimpl.h"
#include "buffer.h"
#include "channel.h"
#include "iocontext.h"
#include "tcp/inetaddress.h"
#include "utils/log.h"
#include <cassert>
#include <cerrno>
#include <cstddef>
#include <fcntl.h>
#include <memory>
#include <string>
#include <sys/types.h>
#include <unistd.h>
namespace tcp
{
Connection::Connection(int clientfd, IoContext* ioContext, std::size_t id, BaseTasks&& baseTasks,
                       ReleaseTask&& releaseTask, InetAddress&& peerAddr, MessageTask&& messageTask)
{
    impl_ = std::make_unique<Impl<Connection>>(clientfd, ioContext, id, std::move(baseTasks), std::move(releaseTask),
                                               std::move(peerAddr), std::move(messageTask));
    impl_->channel_.setReadTask([this]() {
        Logger::logger << ("TcpConnection handleRead: " + std::to_string(impl_->id_));

        if (impl_->readBuffer_.readSocket(impl_->fd_))
        {
            if (impl_->messageTask_)
                impl_->messageTask_(this, impl_->readBuffer_.begin(), impl_->readBuffer_.size());
        }
        else
        {
            stop();
        }
    });
    impl_->channel_.setReadTask([this]() {
        if (impl_->writeBuffer_.writeSocket(impl_->fd_, std::string()))
        {
            if (impl_->writeBuffer_.size() > 0)
            {
                impl_->channel_.setType(Channel::Type::kBoth);
                impl_->ioContext_->updateChannel(&impl_->channel_);
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
        if (impl_->writeBuffer_.writeSocket(impl_->fd_, data))
        {
            if (impl_->writeBuffer_.size() > 0)
            {
                impl_->channel_.setType(Channel::Type::kBoth);
                impl_->ioContext_->updateChannel(&impl_->channel_);
            }
        }
        else
        {
            stop();
        }
    }
}
} // namespace tcp
