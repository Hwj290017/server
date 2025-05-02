#pragma once

#include "sharedobjectid.h"
#include <cstddef>
#include <functional>
namespace tcp
{
class ConnectionId : public SharedObjectId
{
  public:
    using AfterReadTask = std::function<void(ConnectionId, const void*, std::size_t)>;
    using ConnectTask = std::function<void(ConnectionId)>;
    using DisconnectTask = std::function<void(ConnectionId)>;
    ConnectionId(std::size_t id);
    void start(double delay = 0.0);
    void stop(double delay = 0.0);
    void send(const void* data, std::size_t size);
    void setAfterReadTask(AfterReadTask task);
    void setDisconnectTask(ConnectTask task);
    void setConnectTask(DisconnectTask task);
};

} // namespace tcp