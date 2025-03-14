#ifndef CHANNEL_H
#define CHANNEL_H

#include <cstdint>
#include <functional>
class EventLoop;
class Channel
{
  public:
    Channel(int fd, const EventLoop* loop);
    ~Channel();

    int getFd() const;
    uint32_t getEvent() const;
    uint32_t getRevent() const;
    bool isInEpoll() const;
    void setInEpoll();
    void setRevent(uint32_t);
    void setReadCb(std::function<void()>);
    void setWriteCb(std::function<void()>);

    void enableRead();
    void enableWrite();
    void close();

    void handleEvent() const;

  private:
    int fd_;
    const EventLoop* loop_;
    uint32_t event_;
    uint32_t revent_;
    bool inEpoll_;
    std::function<void()> readCb_;
    std::function<void()> writeCb_;
};
#endif