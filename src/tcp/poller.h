#pragma once

#include <utility>
#include <vector>
namespace tcp
{
class Channel;
class Poller
{
  public:
    enum class Type
    {
        kNone = 0,
        kReadable = 1,
        kWriteable = 2,
        kBoth = 3
    };

    using ActiveObj = std::pair<Channel*, Type>;
    virtual std::vector<ActiveObj> poll(int timeout = -1) = 0;
    virtual void update(Channel* channel, Type type) = 0;
    virtual ~Poller() = default;
};
} // namespace tcp
