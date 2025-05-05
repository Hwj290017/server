#pragma once
#include "utils/log.h"
#include <cassert>
#include <functional>
#include <unistd.h>
namespace tcp
{
class IoContext;
class Channel
{
  public:
    using Task = std::function<void()>;
    enum Type
    {
        kNone,
        kReadable,
        kWriteable,
        kBoth
    };
    Channel(int fd) : fd_(fd), type_(kNone), expiredType_(kNone)
    {
    }
    ~Channel()
    {
        assert(fd_ >= 0);
        ::close(fd_);
    }
    void onEvent()
    {
        if ((type_ == kReadable || type_ == kBoth) && readTask_)
        {
            readTask_();
        }
        if ((type_ == kWriteable || type_ == kBoth) && writeTask_)
            writeTask_();
    }
    Type type() const
    {
        return type_;
    }
    Type expiredType() const
    {
        return expiredType_;
    }
    void setType(Type type)
    {
        type_ = type;
    }
    void setExpiredType(Type type)
    {
        expiredType_ = type;
    }

    void setReadTask(Task&& task)
    {
        readTask_ = std::move(task);
    }
    void setWriteTask(Task&& task)
    {
        writeTask_ = std::move(task);
    }

    int fd() const
    {
        return fd_;
    }

  protected:
    int fd_;
    Type type_;
    Type expiredType_;
    Task readTask_;
    Task writeTask_;
};

} // namespace tcp