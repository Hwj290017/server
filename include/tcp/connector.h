#pragma once
#include "tcp/baseobject.h"
#include "tcp/inetaddress.h"
#include "tcp/tempptr.h"
#include <cstddef>
#include <functional>

namespace tcp
{
class Connector : public BaseObject<Connector>
{
  public:
    using MessageTask = std::function<void(const TempPtr<Connector>&, const void*, std::size_t)>;
    struct Tasks
    {
        StartTask startTask;
        StopTask stopTask;
        MessageTask messageTask;
    };
    explicit Connector(IoContext* ioContext, std::size_t id, const InetAddress& serverAddr, const Tasks& tasks,
                       const ReleaseTask& releaseTask);
    ~Connector();
    void send(const void* data, std::size_t size);
    void send(const std::string& data);
};

} // namespace tcp