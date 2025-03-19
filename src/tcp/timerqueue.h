#ifndef TIMERQUEUE_H
#define TIMERQUEUE_H
#include "channel.h"
#include "timer.h"
#include <cstdint>
#include <ctime>
#include <functional>
#include <map>
#include <memory>
#include <set>

class EventLoop;

class TimerQueue
{
  public:
    typedef std::function<void()> Task;
    typedef std::unique_ptr<Timer> TimerPtr;
    typedef std::pair<TimeSpec, uint64_t> TimerIndex;

    TimerQueue(EventLoop* loop);
    ~TimerQueue();
    TimerId addTimer(const Task& cb, const TimeSpec& when, double interval);
    void removeTimer(TimerId& timerId);
    void updateTimerFd();

  private:
    EventLoop* loop_;
    int timefd_;
    Channel channel_;
    // 下次触发时间，就是文件描述符的到期时间
    TimeSpec nextExpire_;
    bool callingExpiredTimers_;
    // 存放所有定时器
    std::map<TimerIndex, TimerPtr> timers_;
    // 存放在执行handleRead中删除的定时器
    std::set<uint64_t> removedTimers_;

    void handleRead();

    // 由addTimer插入到loop队列中
    void addTimerInLoop(Timer* timer);
    void removeTimerInLoop(TimerId timerId);
    void getExpiredTimers(const TimeSpec& now, std::vector<TimerPtr>& expiredTimers);
    static int createTimerFd();
};

#endif // TIMEQUEUE_H