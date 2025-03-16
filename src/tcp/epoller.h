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
    void poll(std::vector<Channel*>& activeChannels, int timeout) override;
    // 注册Channel
    void addChannel(Channel* channel) override;
    // 更新Channel
    void updateChannel(Channel* channel) override;
    // 删除Channel
    void removeChannel(Channel* channel) override;
    // 判断Channel是否注册
    bool hasChannel(Channel* channel) override;

  private:
    int epfd_;
    epoll_event* events_;
    std::unordered_map<int, Channel*> channels_;
};
