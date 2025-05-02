#pragma once

#include "tcp/sharedobjectid.h"
#include <cstddef>
#include <memory>

namespace tcp
{
class AcceptorId : public SharedObjectId
{
  public:
    ~AcceptorId();
    void start(double delay = 0.0);
    void stop(double delay = 0.0);
    void isStopped();
    std::size_t id() const;

  private:
    AcceptorId(std::size_t id);
    friend class Server;
};

} // namespace tcp