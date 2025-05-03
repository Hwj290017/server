
#include "poller.h"
#include <cstddef>
#include <sys/epoll.h>
#include <unordered_map>
#include <vector>
namespace tcp
{
class Epoller : public Poller
{
  public:
    Epoller();
    ~Epoller() override;
    auto poll(int timeout = -1) -> std::vector<ActiveObj> override;
    void update(Channel* channel, Type type) override;

  private:
    epoll_event getEvent(Type type);
    static constexpr std::size_t kEventSize = 1024;
    int epfd_;
    epoll_event events_[kEventSize];
    std::unordered_map<int, Type> attachedFds_;
};
} // namespace tcp
