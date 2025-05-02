#pragma once
#include "connectionid.h"
#include "tcp/acceptorid.h"
#include "tcp/connectorid.h"
namespace tcp
{
class InetAddress;
class Server
{
  public:
    void start();
    AcceptorId newAcceptor(const InetAddress& listenAddr);
    ConnectorId newConnector(const InetAddress& serverAddr);
    void setAfterReadTask(const ConnectionId::AfterReadTask& task);
    void setDisconnectTask(const ConnectionId::ConnectTask& task);
    void setConnectTask(const ConnectionId::DisconnectTask& task);
    void setAfterReadTask(const ConnectorId::AfterReadTask& task);
    void setDisconnectTask(const ConnectorId::ConnectTask& task);
    void setConnectTask(const ConnectorId::DisconnectTask& task);
};

} // namespace tcp