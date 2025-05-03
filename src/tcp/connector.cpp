#include "connector.h"
#include "buffer.h"
#include "iocontext.h"
#include "sharedobjectpool.h"
#include "socket.h"
#include "tcp/connectorid.h"
#include "util/log.h"
namespace tcp
{
Connector::Connector(IoContext* ioContext, std::size_t id, const InetAddress& serverAddr)
    : SharedObject(socket::createConnectorSocket(serverAddr), ioContext, id), serverAddr_(serverAddr),
      state_(State::kConnected)
{
}

Connector::~Connector() = default;

void Connector::start()
{
    if (connectTask_)
        connectTask_(ConnectorId(id_));
    ioContext_->updateObject(this, Poller::Type::kReadable);
}

void Connector::stop()
{
    if (state_ == State::kConnected)
    {
        state_ = State::kDisconnected;
        ioContext_->queueTask([this]() {
            if (disconnectTask_)
                disconnectTask_(ConnectorId(id_));
            ioContext_->updateObject(this, Poller::Type::kNone);
            SharedObjectPool::instance().releaseObject(id_);
        });
    }
}

void Connector::send(std::string&& data)
{
    if (data.length() > 0)
    {
        if (writeBuffer_.writeSocket(fd_, data))
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
}

void Connector::onWrite()
{
    // Logger::logger << ("TcpConnector handleWrite: " + id() + "\nWriteNum: " + std::to_string(writeNum));

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

void Connector::onRead()
{

    Logger::logger << ("TcpConnector handleRead: " + std::to_string(id_));

    if (readBuffer_.readSocket(fd_))
    {
        if (afterReadTask_)
            afterReadTask_(ConnectorId(id_), readBuffer_.begin(), readBuffer_.size());
    }
    else
    {
        stop();
    }
}

void Connector::setConnectTask(ConnectorId::ConnectTask&& task)
{
    connectTask_ = std::move(task);
}

void Connector::setDisconnectTask(ConnectorId::DisconnectTask&& task)
{
    disconnectTask_ = std::move(task);
}

void Connector::setAfterReadTask(ConnectorId::AfterReadTask&& task)
{
    afterReadTask_ = std::move(task);
}
} // namespace tcp