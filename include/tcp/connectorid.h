#pragma once

#include "tcp/sharedobjectid.h"
#include <any>
#include <cstddef>
#include <functional>
#include <memory>
namespace tcp
{
class ConnectorId : public SharedObjectId
{
  public:
    using ConnectTask = std::function<void(const ConnectorId&)>;
    using AfterReadTask = std::function<void(const ConnectorId&)>;
    using DisconnectTask = std::function<void(const ConnectorId&)>;
    void start(double delay = 0.0);
    void stop(double delay = 0.0);
    void send(const void* data, size_t len) const;
    void setContext(std::any context);
    std::any& getContext();
    void setAfterReadTask(AfterReadTask task);
    void setDisconnectTask(ConnectTask task);
    void setConnectTask(DisconnectTask task);

  private:
    ConnectorId(std::size_t id);
    friend class Server;
};

} // namespace tcp