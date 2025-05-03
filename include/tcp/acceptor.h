#pragma once

#include "object.h"
#include "tcp/connection.h"
#include "tcp/inetaddress.h"
#include "tcp/tempptr.h"
#include <cstddef>
#include <functional>
namespace tcp
{
class Acceptor : public Object
{
  public:
    // 在接收连接是获取连接的回调,返回值表示是否接收连接
    using StartTask = std::function<void(TempPtr<Acceptor>)>;
    using AcceptTask = std::function<bool(TempPtr<Acceptor>, const InetAddress& peerAddr, tcp::Connection::Tasks*)>;
    using StopTask = std::function<void(TempPtr<Acceptor>)>;

    struct Tasks
    {
        StartTask startTask;
        AcceptTask acceptTask;
        StopTask stopTask;
    };
    ~Acceptor();

  private:
    using NewConnectionTask = std::function<void(int, const InetAddress&)>;
    using ReleaseTask = std::function<void(std::size_t)>;
    explicit Acceptor(const InetAddress& listenAddr, const AcceptTask& acceptTask,
                      const NewConnectionTask& newConnectionTask, const ReleaseTask& releaseTask);
    struct Impl;
    std::unique_ptr<Impl> impl_;
    friend class Manager;
};

} // namespace tcp