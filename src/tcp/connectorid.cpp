#include "tcp/connectorid.h"
#include "connector.h"
#include "iocontext.h"
#include "sharedobjectpool.h"
namespace tcp
{
ConnectorId::ConnectorId(std::size_t id) : SharedObjectId(id)
{
}

void ConnectorId::send(const void* data, std::size_t size)
{
    if (isAlive())
    {
        auto ioContext = SharedObjectPool::instance().getIoContext(id_);
        ioContext->runTask([id = id_, data = std::string(static_cast<const char*>(data), size)]() mutable {
            auto obj = SharedObjectPool::instance().getObject(id);
            if (obj)
                static_cast<Connector*>(obj)->send(std::move(data));
        });
    }
}

void ConnectorId::setAfterReadTask(const AfterReadTask& task)
{
    if (isAlive())
    {
        auto ioContext = SharedObjectPool::instance().getIoContext(id_);
        ioContext->runTask([id = id_, task = task]() mutable {
            auto obj = SharedObjectPool::instance().getObject(id);
            if (obj)
                static_cast<Connector*>(obj)->setAfterReadTask(std::move(task));
        });
    }
}

void ConnectorId::setConnectTask(const ConnectTask& task)
{
    if (isAlive())
    {
        auto ioContext = SharedObjectPool::instance().getIoContext(id_);
        ioContext->runTask([id = id_, task = task]() mutable {
            auto obj = SharedObjectPool::instance().getObject(id);
            if (obj)
                static_cast<Connector*>(obj)->setConnectTask(std::move(task));
        });
    }
}

void ConnectorId::setDisconnectTask(const DisconnectTask& task)
{
    if (isAlive())
    {
        auto ioContext = SharedObjectPool::instance().getIoContext(id_);
        ioContext->runTask([id = id_, task = task]() mutable {
            auto obj = SharedObjectPool::instance().getObject(id);
            if (obj)
                static_cast<Connector*>(obj)->setDisconnectTask(std::move(task));
        });
    }
}
} // namespace tcp