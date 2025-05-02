#pragma once
#include "tcp/iocontextid.h"
#include <cstddef>
namespace tcp
{

class SharedObjectId
{
  public:
    SharedObjectId(std::size_t id);
    void start(double delay = 0.0);
    void stop(double delay = 0.0);
    // 不实时精确
    bool isAlive() const;
    SharedObjectId parent() const;
    IoContextId IoContextId() const;

  protected:
    std::size_t id_;
    std::size_t IoContextId_;
};

} // namespace tcp