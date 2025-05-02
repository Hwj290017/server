#pragma once

#include <cstddef>
namespace tcp
{

class SharedObjectId
{
  protected:
    SharedObjectId(std::size_t id);
    std::size_t id_;
};

} // namespace tcp