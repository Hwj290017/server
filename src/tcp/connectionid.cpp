#include "tcp/connectionid.h"
#include "tcp/sharedobjectid.h"

namespace tcp
{
ConnectionId::ConnectionId(std::size_t id, std::size_t parent) : SharedObjectId(id, parent)
{
}
} // namespace tcp