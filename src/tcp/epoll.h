

#ifndef EPOLL_H
#define EPOLL_H

#include <cstdint>
#include <vector>

#define MAX_EVENTS 1024

class Channel;
class epoll_event;
class Epoll
{
  private:
    int epfd;
    epoll_event* events;

  public:
    Epoll();
    ~Epoll();

    void addFd(int fd, uint32_t op) const;
    void updateChannel(Channel* channel) const;
    std::vector<Channel*> poll(int timeout = -1) const;
};
#endif