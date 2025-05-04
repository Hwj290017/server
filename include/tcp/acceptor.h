#pragma once
#include "tcp/baseobject.h"
#include "tcp/connection.h"
#include "tcp/inetaddress.h"
#include "tcp/tempptr.h"
#include <cstddef>
#include <functional>
namespace tcp
{
class Acceptor : public BaseObject<Acceptor>
{
  public:
    // 在接收连接是获取连接的回调,返回值表示是否接收连接
    using AcceptTask = std::function<void(TempPtr<Acceptor>, int clientfd, const InetAddress& peerAddr)>;
    ~Acceptor();

  private:
    // 由server负责实例化
    explicit Acceptor(IoContext* ioContext, std::size_t id, BaseTasks&& baseTasks, ReleaseTask&& releaseTask,
                      InetAddress&& listenAddr, AcceptTask&& acceptTask);
    Acceptor(Acceptor&&) noexcept;
    friend class BaseObjectPool;
};

extern template class BaseObject<Acceptor>;
} // namespace tcp