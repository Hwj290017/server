#include "rwableFd.h"
#include <unistd.h>

RWAbleFd::~RWAbleFd()
{
    if (fd_ >= 0)
    {
        ::close(fd_);
        fd_ = -1;
    }
}

int RWAbleFd::read(void* buf, size_t len) const
{
    return ::read(fd_, buf, len);
}

int RWAbleFd::write(const void* buf, size_t len) const
{
    return ::write(fd_, buf, len);
}

int RWAbleFd::write(const std::string& buf) const
{
    return write(static_cast<const void*>(buf.data()), buf.length());
}