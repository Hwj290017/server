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
    using MessageTask = std::function<void(const TempPtr<Connection>&, const void*, std::size_t)>;
    struct Tasks
    {
        StartTask startTask;
        StopTask stopTask;
        MessageTask messageTask;
    };
    // 由server实例化
    explicit Connection(int clientfd, IoContext* ioContext, std::size_t id, const InetAddress& peerAddr,
                        const Tasks& tasks, const ReleaseTask& releaseTask);
    Connection(Connection&& other) noexcept;
    ~Connection();
    // 需要考虑线程安全问题
    void send(const std::string& data);
    void send(std::string&& data);
    void send(const void* data, std::size_t len);
};

} // namespace tcp