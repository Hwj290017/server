#pragma once

#include "tcp/sharedobjectid.h"
#include <any>
#include <cstddef>
#include <functional>

namespace tcp
{
class ConnectorId : public SharedObjectId
{
  public:
    using ConnectTask = std::function<void(const ConnectorId&)>;
    using AfterReadTask = std::function<void(const ConnectorId&, const void*, std::size_t)>;
    using DisconnectTask = std::function<void(const ConnectorId&)>;

    ConnectorId(std::size_t id);
    void start(double delay = 0.0);
    void stop(double delay = 0.0);
    void send(const void* data, size_t len);
    void setContext(std::any context);
    std::any& getContext();
    void setAfterReadTask(const AfterReadTask& task);
    void setDisconnectTask(const ConnectTask& task);
    void setConnectTask(const DisconnectTask& task);
};

} // namespace tcp