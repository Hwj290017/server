#include "tcp/acceptorId.h"
#include "iocontextpool.h"
#include "sharedobjectpool.h"
#include "tcp/iocontextid.h"
#include <cstddef>
#include <memory>

namespace tcp
{
struct AcceptorId::Impl
{
    Impl(const InetAddress& listenAddr, std::size_t ioContextId)
        : id_(SharedObjectPool::instance().getAcceptor(listenAddr, ioContextId)), ioContextId_(ioContextId)
    {
    }
    std::size_t id_;
    std::size_t ioContextId_;
};
AcceptorId::AcceptorId(const InetAddress& listenAddr, IoContextId ioContextId)
    : impl_(std::make_unique<Impl>(listenAddr, ioContextId.id()))
{
}

void AcceptorId::start(double delay)
{
    IoContextPool::instance().getIoContext(impl_->ioContextId_)->start(impl_->id_, delay);
}

void AcceptorId::stop(double delay)
{
    IoContextPool::instance().getIoContext(impl_->ioContextId_)->stop(impl_->id_, delay);
}
} // namespace tcp