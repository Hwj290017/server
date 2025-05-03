#pragma once

#include <utility>
#include <vector>
namespace tcp
{
class Channel;
class Poller
{
  public:
    virtual std::vector<Channel*> poll(int timeout = -1) = 0;
    virtual void update(Channel* channel) = 0;
    virtual ~Poller() = default;
};
} // namespace tcp
