
#include "sharedobject.h"
#include "object.h"
#include <cstddef>

namespace tcp
{

SharedObject::SharedObject(int fd, IoContext* ioContext, std::size_t id) : Object(fd, ioContext), id_(id) {};

} // namespace tcp