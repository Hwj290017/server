#pragma once

#include "tcp/sharedobjectid.h"
#include <cstddef>
#include <memory>

namespace tcp
{
class AcceptorId : public SharedObjectId
{
  public:
    AcceptorId(std::size_t id);
    ~AcceptorId();
};

} // namespace tcp