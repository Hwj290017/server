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
} // namespace tcp