#pragma once
#include "tcp/acceptorid.h"
#include "tcp/connectionid.h"
#include "tcp/connectorid.h"
#include <cstddef>
namespace tcp
{
class InetAddress;
class Server
{
  public:
    void start();
    AcceptorId newAcceptor(const InetAddress& listenAddr, std::size_t parent = 0);
    ConnectorId newConnector(const InetAddress& serverAddr, std::size_t parent = 0);
    void setAfterReadTask(const ConnectionId::AfterReadTask& task);
    void setDisconnectTask(const ConnectionId::ConnectTask& task);
    void setConnectTask(const ConnectionId::DisconnectTask& task);
    void setAfterReadTask(const ConnectorId::AfterReadTask& task);
    void setDisconnectTask(const ConnectorId::ConnectTask& task);
    void setConnectTask(const ConnectorId::DisconnectTask& task);
};

} // namespace tcp