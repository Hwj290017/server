#ifndef CHANNEL_H
#define CHANNEL_H

#include <cstdint>
#include <functional>

class Channel
{
  public:
    Channel(int fd);
    ~Channel();

    void enableRead();
    void enableWrite();
    void enableEt();

    int getFd() const;
    uint32_t getEvent() const;
    uint32_t getRevent() const;
    void setRevent(uint32_t);
    void setReadCb(std::function<void()>);
    void setWriteCb(std::function<void()>);
    void handleEvent() const;

  private:
    int fd_;
    uint32_t event_;
    uint32_t revent_;
    std::function<void()> readCb_;
    std::function<void()> writeCb_;
};
#endif