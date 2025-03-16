#ifndef POLLER_H
#define POLLER_H
#include <vector>
class Channel;
class Poller
{
  public:
    // 获取活跃的Channel
    virtual void poll(std::vector<Channel*>& activeChannels, int timeout) = 0;
    // 注册Channel
    virtual void addChannel(Channel* channel) = 0;
    // 更新Channel
    virtual void updateChannel(Channel* channel) = 0;
    // 删除Channel
    virtual void removeChannel(Channel* channel) = 0;
    // 判断Channel是否注册
    virtual bool hasChannel(Channel* channel) = 0;
    virtual ~Poller() = default;
};

#endif // POLLER_H