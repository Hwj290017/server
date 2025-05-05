#pragma once
#include "tcp/baseobject.h"
#include "tcp/connection.h"
#include "tcp/inetaddress.h"
#include "tcp/tempptr.h"
#include <cstddef>
#include <functional>
#include <memory>
namespace tcp
{
class Acceptor : public BaseObject<Acceptor>
{
  public:
    // 在接收连接是获取连接的回调,返回值表示是否接收连接
    using AcceptTask = std::function<void(const TempPtr<Acceptor>&, int clientfd, const InetAddress& peerAddr)>;
    struct Tasks
    {
        StartTask startTask;
        StopTask stopTask;
        AcceptTask acceptTask;
    };
    explicit Acceptor(IoContext* ioContext, std::size_t id, const InetAddress& listenAddr, const Tasks& tasks,
                      const ReleaseTask& releaseTask);
    ~Acceptor();
};

extern template class BaseObject<Acceptor>;
} // namespace tcp