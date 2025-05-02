#pragma once

#include "tcp/inetAddress.h"
#include "tcp/iocontextid.h"
#include <cstddef>
#include <memory>

namespace tcp
{
class AcceptorId
{
  public:
    AcceptorId(const InetAddress& listenAddr, IoContextId ioContextId);
    ~AcceptorId();
    void start(double delay = 0.0);
    void stop(double delay = 0.0);
    std::size_t id() const;

  private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace tcp