#pragma once
#include "tcp/baseobject.h"
#include "tcp/inetaddress.h"
#include "tcp/tempptr.h"
#include <cstddef>
#include <functional>
#include <memory>
namespace tcp
{
class Connection : public BaseObject<Connection>
{
  public:
    using MessageTask = std::function<void(TempPtr<Connection>, const void*, std::size_t)>;
    struct Tasks
    {
        StartTask startTask;
        StopTask stopTask;
        MessageTask messageTask;
    };
    ~Connection();
    // 需要考虑线程安全问题
    void send(const std::string& data);
    void send(std::string&& data);
    void send(const void* data, std::size_t len);

  private:
    // 由server实例化
    explicit Connection(int clientfd, IoContext* ioContext, std::size_t id, InetAddress&& peerAddr, Tasks&& tasks,
                        ReleaseTask&& releaseTask);
    friend class BaseObjectPool;
};

} // namespace tcp