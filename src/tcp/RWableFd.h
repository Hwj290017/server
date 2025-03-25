#ifndef RWABLE_FD_H
#define RWABLE_FD_H
#include <cstddef>
class RWAbleFd
{
  public:
    RWAbleFd(int fd) : fd_(fd)
    {
    }
    RWAbleFd(const RWAbleFd&) = delete;
    RWAbleFd(RWAbleFd&& other) : fd_(other.fd_)
    {
        other.fd_ = -1;
    }
    RWAbleFd& operator=(const RWAbleFd&) = delete;
    RWAbleFd& operator=(RWAbleFd&& other) = delete;

    virtual ~RWAbleFd();
    int fd() const
    {
        return fd_;
    }
    int read(void* buf, size_t len) const;
    int write(const void* buf, size_t len) const;

  protected:
    int fd_;
};
#endif