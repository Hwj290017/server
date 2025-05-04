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
    explicit Connector(IoContext* ioContext, std::size_t id, BaseTasks&& baseTasks, ReleaseTask&& releaseTask,
                       InetAddress&& serverAddr, MessageTask&& messageTask);
    friend class BaseObjectPool;
};

} // namespace tcp