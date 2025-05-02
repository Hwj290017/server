#include "tcp/sharedobjectid.h"
#include "iocontext.h"
#include "sharedobjectpool.h"
#include <cstddef>
namespace tcp
{
SharedObjectId::SharedObjectId(std::size_t id) : id_(id)
{
}

bool SharedObjectId::isAlive() const
{
    return SharedObjectPool::instance().getObject(id_) != nullptr;
}

SharedObjectId SharedObjectId::parent() const
{
    return SharedObjectPool::instance().getParent(id_)
}

void SharedObjectId::start(double delay)
{
    auto ioContext = SharedObjectPool::instance().getIoContext(id_);
    ioContext->runTask(
        [id = id_]() {
            auto obj = SharedObjectPool::instance().getObject(id);
            if (obj)
                obj->start();
        },
        delay);
}

void SharedObjectId::stop(double delay)
{
    auto ioContext = SharedObjectPool::instance().getIoContext(id_);
    ioContext->runTask(
        [id = id_]() {
            auto obj = SharedObjectPool::instance().getObject(id);
            if (obj)
                obj->stop();
        },
        delay);
}
} // namespace tcp