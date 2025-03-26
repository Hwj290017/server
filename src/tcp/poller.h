#ifndef POLLER_H
#define POLLER_H
#include <vector>
class Channel;
class Poller
{
  public:
    // 获取活跃的Channel
    virtual std::vector<Channel*> poll(int timeout = -1) = 0;
    // 更新Channel
    virtual void updateChannel(Channel* channel) = 0;
    // 判断Channel是否注册
    virtual bool hasChannel(const Channel* channel) = 0;
    virtual ~Poller() = default;
};

#endif // POLLER_H