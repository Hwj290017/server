#pragma once
#include "tcp/baseobject.h"
#include "tcp/inetaddress.h"
#include "tcp/tempptr.h"
#include <cstddef>
#include <functional>
#include <memory>

namespace tcp
{
class Connector : public BaseObject<Connector>
{
  public:
    using MessageTask = std::function<void(TempPtr<Connector>, const void*, std::size_t)>;

    ~Connector();
    void send(const void* data, std::size_t size);

  private:
    // 由server实例化
    explicit Connector(IoContext* ioContext, std::size_t id, const InetAddress& serverAddr, const BaseTasks& baseTasks,
                       const ReleaseTask& releaseTask);
    friend class Server;
};

} // namespace tcp