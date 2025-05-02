#pragma once
#include "object.h"
#include "tcp/sharedobjectid.h"
#include <cstddef>

namespace tcp
{
class Poller;
class SharedObject : public Object
{
  public:
    SharedObject(int fd, IoContext* ioContext, std::size_t id);
    ~SharedObject();
    virtual void start();
    virtual void stop();

  protected:
    std::size_t id_;
};
} // namespace tcp
