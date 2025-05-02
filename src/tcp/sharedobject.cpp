
#include "sharedobject.h"
#include "object.h"
#include <cstddef>

namespace tcp
{

SharedObject::SharedObject(int fd, IoContext* ioContext, std::size_t id, std::size_t parent)
    : Object(fd, ioContext), id_(id), parent_(parent) {};

} // namespace tcp