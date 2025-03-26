#ifndef TIMER_H
#define TIMER_H

#include <cstdint>
#include <ctime>
#include <functional>

class Timer;
class TimeSpec;
class TimerId
{
  public:
    TimerId(const TimerId& other) : timer_(other.timer_), id_(other.id_)
    {
    }
    // 由调用者保证线程安全
    // void setTime(const TimeSpec& time);

  private:
    TimerId(Timer* timer, uint64_t id) : timer_(timer), id_(id)
    {
    }

    Timer* timer_;
    uint64_t id_;
    friend class TimerQueue;
};

// 加入比较函数
class TimeSpec : public timespec
{
  public:
    TimeSpec() : timespec()
    {
    }
    TimeSpec(const timespec& ts) : timespec(ts)
    {
    }

    bool operator<(const TimeSpec& other) const
    {
        if (tv_sec != other.tv_sec)
            return tv_sec < other.tv_sec;
        else
            return tv_nsec < other.tv_nsec;
    }
    bool operator!=(const TimeSpec& other) const
    {
        return tv_sec != other.tv_sec || tv_nsec != other.tv_nsec;
    }

    bool operator==(const TimeSpec& other) const
    {
        return !(*this != other);
    }
    TimeSpec operator+(const TimeSpec& other) const
    {
        return TimeSpec({tv_sec + other.tv_sec, tv_nsec + other.tv_nsec});
    }

    TimeSpec operator+(double delay) const
    {
        return TimeSpec({tv_sec + static_cast<time_t>(delay),
                         tv_nsec + static_cast<long>((delay - static_cast<time_t>(delay)) * 1e9) % 1000000000});
    }
    // 获取当前时间
    static TimeSpec getNow()
    {
        TimeSpec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return now;
    }
    // 无效时间
    const static TimeSpec inValidExpired;
};

class Timer
{
  public:
    Timer(const std::function<void()>& cb, TimeSpec expired, double interval)
        : cb(cb), expired(expired), interval(interval), id(count++)
    {
    }

    void restart()
    {
        expired.tv_sec += interval;
    }
    TimeSpec getExpired() const
    {
        return expired;
    }
    bool isRepeat() const
    {
        return interval > 0;
    }
    void handleEvent() const
    {
        if (cb)
            cb();
    }
    uint64_t getId() const
    {
        return id;
    }

  private:
    const std::function<void()> cb;
    TimeSpec expired;
    const double interval;
    const uint64_t id;
    static uint64_t count;
};

#endif // TIMER_H