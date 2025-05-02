#pragma once

#include <cassert>
#include <sys/types.h>
#include <unistd.h>
namespace tcp
{

class IoContext;
class Object
{
  public:
    virtual void onRead() {};
    virtual void onWrite() {};
    int fd() const;

  protected:
    Object(int fd, IoContext* ioContext) : fd_(fd), ioContext_(ioContext)
    {
    }
    ~Object()
    {
        assert(fd_ >= 0);
        ::close(fd_);
    }
    int fd_;
    IoContext* ioContext_;
};

} // namespace tcp