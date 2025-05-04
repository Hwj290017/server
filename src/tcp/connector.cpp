#include "tcp/connector.h"
#include "baseobjectimpl.h"
#include "buffer.h"
#include "iocontext.h"
#include "socket.h"
#include "utils/log.h"
#include <memory>
namespace tcp
{
Connector::Connector(IoContext* ioContext, std::size_t id, BaseTasks&& baseTasks, ReleaseTask&& releaseTask,
                     InetAddress&& serverAddr, MessageTask&& messageTask)
{
    impl_ = std::make_unique<Impl<Connector>>(socket::createConnectorSocket(serverAddr), ioContext, id,
                                              std::move(baseTasks), std::move(releaseTask), std::move(serverAddr),
                                              std::move(messageTask));
    impl_->channel_.setReadTask([this]() {
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

Connector::~Connector() = default;

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