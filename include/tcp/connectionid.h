#pragma once
#include "tcp/sharedobjectid.h"
#include <cstddef>
#include <functional>
#include <memory>
namespace tcp
{
class ConnectionId : public SharedObjectId
{
  public:
    using AfterReadTask = std::function<void(ConnectionId, const void*, std::size_t)>;
    using ConnectTask = std::function<void(ConnectionId)>;
    using DisconnectTask = std::function<void(ConnectionId)>;

    ConnectionId(std::size_t id);
    void send(const void* data, std::size_t size);
    void setAfterReadTask(AfterReadTask task);
    void setDisconnectTask(ConnectTask task);
    void setConnectTask(DisconnectTask task);
};

} // namespace tcp