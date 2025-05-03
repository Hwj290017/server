#pragma once
#include <cassert>
#include <unistd.h>
namespace tcp
{
class IoContext;
class Channel
{
  public:
    virtual void onRead()
    {
    }
    virtual void onWrite()
    {
    }
    int fd();

  protected:
    Channel(int fd) : fd_(fd)
    {
    }
    ~Channel()
    {
        assert(fd_ >= 0);
        ::close(fd_);
    }
    int fd_;
};

} // namespace tcp