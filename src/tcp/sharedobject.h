#pragma once
#include "object.h"
#include <cstddef>

namespace tcp
{
class Poller;
class SharedObject : public Object
{
  public:
    SharedObject(int fd, IoContext* ioContext, std::size_t id, std::size_t parent);
    ~SharedObject();
    virtual void start();
    virtual void stop();

  protected:
    std::size_t id_;
    std::size_t parent_;
};
} // namespace tcp
