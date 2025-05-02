#pragma once

#include "tcp/inetAddress.h"

namespace tcp
{
class SharedObject;
class SharedObjectPool
{
  public:
    SharedObjectPool();
    std::size_t getAcceptor(const InetAddress& listenAddr, std::size_t ioContextid);
    std::size_t getConnection(int clientSocket, const InetAddress& peerAddr, std::size_t ioContextid);
    std::size_t getConnector(const InetAddress& serverAddr, std::size_t ioContextid);
    SharedObject* getObject(std::size_t id);
    static SharedObjectPool& instance();
};

} // namespace tcp