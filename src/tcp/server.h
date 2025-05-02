#pragma once

#include "acceptorId.h"
#include "connectionid.h"
#include "connectorid.h"
#include "inetAddress.h"
#include "iocontextpool.h"
#include "sharedobject.h"
#include "sharedobjectid.h"
#include <memory>
namespace tcp
{

class Server
{
  public:
    auto newAcceptor(const InetAddress& listenAddr, SharedObjectId parent = SharedObjectId()) -> AcceptorId;
    void start();
    auto newConnection(int clientSocket, const InetAddress& peerAddr, SharedObjectId parent = SharedObjectId())
        -> ConnectionId;
    auto newConnector(const InetAddress& serverAddr, SharedObjectId parent = SharedObjectId()) -> ConnectorId;
    static Server& instance();

  private:
    Server();
    ~Server();
    IoContextPool ioContextpool_;
};

} // namespace tcp
