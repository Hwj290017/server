#include "connection.h"
#include "iocontext.h"
#include "object.h"
#include "poller.h"
#include "sharedobject.h"
#include "sharedobjectpool.h"
#include "tcp/connectionid.h"
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
Connection::Connection(int clientSocket, IoContext* ioContext, std::size_t id, const InetAddress& clientAddr)
    : SharedObject(clientSocket, ioContext, id), addr_(clientAddr), state_(kConnected)
{
}

Connection::~Connection() = default;

void Connection::start()
{
    if (connectTask_)
        connectTask_(ConnectionId(id_));
    ioContext_->updateObject(this, Poller::Type::kReadable);
}

void Connection::stop()
{
    if (state_ == kConnected)
    {
        state_ = kDisconnected;
        // 放在队列最后执行
        ioContext_->queueTask([this]() {
            if (disconnectTask_)
                disconnectTask_(ConnectionId(id_));
            ioContext_->updateObject(this, Poller::Type::kNone);
            SharedObjectPool::instance().releaseObject(id_);
        });
    }
}

void Connection::send(std::string&& data)
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
