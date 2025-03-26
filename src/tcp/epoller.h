#include "channel.h"
#include "poller.h"
#include <sys/epoll.h>
#include <unordered_map>
#include <vector>

#define MAX_EVENTS 1024
class Epoller : public Poller
{
  public:
    Epoller();
    ~Epoller() override;
    // 获取活跃的Channel
    std::vector<Channel*> poll(int timeout = -1) override;
    // 更新Channel
    void updateChannel(Channel* channel) override;
    // 判断Channel是否注册
    bool hasChannel(const Channel* channel) override;

  private:
    int epfd_;
    epoll_event events_[MAX_EVENTS];
    std::unordered_map<int, Channel*> channels_;
    static int createEpollFd_();
};
