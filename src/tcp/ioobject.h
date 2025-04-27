#pragma once

#include <atomic>
#include <cassert>
#include <cstddef>
#include <sys/types.h>
#include <unistd.h>
namespace tcp
{
class IoContext;

class IoObject
{
  public:
    virtual void onRead() {};
    virtual void onWrite() {};
    int fd() const;
    std::size_t id() const;

  protected:
    IoObject(int fd, IoContext* context) : fd_(fd), ioContext_(context), id_(nextId_++)
    {
    }
    ~IoObject()
    {
        assert(fd_ >= 0);
        ::close(fd_);
    }
    int fd_;
    IoContext* ioContext_;

  private:
    std::size_t id_;
    static std::atomic<std::size_t> nextId_;
};

} // namespace tcp