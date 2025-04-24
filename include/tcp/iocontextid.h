#pragma once

#include "tcp/acceptorId.h"
#include "tcp/connectionid.h"
#include "tcp/connectorid.h"
#include <memory>
#include <vector>
namespace tcp
{

class IoContextId
{
  public:
    using IoContextTask = std::function<void(IoContextId)>;

    std::vector<AcceptorId> getAllAcceptorId() const;
    std::vector<ConnectionId> getAllConnectionId() const;
    std::vector<ConnectorId> getAllConnectorId() const;
    void addAcceptor(AcceptorId id);
    void removeAcceptor(AcceptorId id);
    void addConnector(ConnectorId id);
    void removeConnector(ConnectorId id);
    void doTask(IoContextId);

  private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace tcp