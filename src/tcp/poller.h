#pragma once

#include <utility>
#include <vector>
namespace tcp
{
class Poller
{
  public:
    enum class Type
    {
        kReadable,
        kWriteable,
        kBoth,
        kStopped,
        kNone
    };

    using ActiveObj = std::pair<void*, Type>;
    virtual std::vector<ActiveObj> poll(int timeout = -1) = 0;
    virtual void update(int fd, void* data, Type type) = 0;
    virtual ~Poller() = default;
};
} // namespace tcp
