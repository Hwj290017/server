#pragma once

#include "tcp/inetaddress.h"

namespace tcp
{
class SharedObject;
class IoContext;
class SharedObjectPool
{
  public:
    SharedObjectPool();
    std::size_t getAcceptor(const InetAddress& listenAddr, IoContext* ioContext);
    std::size_t getConnection(int clientSocket, const InetAddress& peerAddr, IoContext* ioContext);
    std::size_t getConnector(const InetAddress& serverAddr, IoContext* ioContext);
    SharedObject* getObject(std::size_t id);
    IoContext* getIoContext(std::size_t id);
    SharedObject* getParent(std::size_t id);
    static SharedObjectPool& instance();
};

} // namespace tcp