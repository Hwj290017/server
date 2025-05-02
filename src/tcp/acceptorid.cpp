#include "tcp/acceptorid.h"
#include "acceptor.h"
#include "iocontext.h"

#include "sharedobjectpool.h"
#include <cstddef>

namespace tcp
{

AcceptorId::AcceptorId(std::size_t id) : SharedObjectId(id)
{
}

void AcceptorId::start(double delay)
{
    auto ioContext = SharedObjectPool::instance().getIoContext(id_);
    ioContext->runTask([id = id_]() {
        auto obj = SharedObjectPool::instance().getObject(id);
        if (obj)
            static_cast<Acceptor*>(obj)->start();
    });
}

void AcceptorId::stop(double delay)
{
    auto ioContext = SharedObjectPool::instance().getIoContext(id_);
    ioContext->runTask([id = id_]() {
        auto obj = SharedObjectPool::instance().getObject(id);
        if (obj)
            static_cast<Acceptor*>(obj)->stop();
    });
}
} // namespace tcp