#pragma once
#include "tcp/inetaddress.h"
#include "tcp/sharedobjectid.h"
#include <cstddef>

namespace tcp
{
class SharedObject;
class IoContext;
class SharedObjectPool
{
  public:
    SharedObjectPool();
    std::size_t getAcceptor(const InetAddress& listenAddr, std::size_t parent);
    std::size_t getConnection(int clientSocket, const InetAddress& peerAddr, std::size_t parent);
    std::size_t getConnector(const InetAddress& serverAddr, std::size_t parent);
    SharedObject* getObject(std::size_t id);
    IoContext* getIoContext(std::size_t id);
    SharedObjectId getParent(std::size_t id);
    void releaseObject(std::size_t id);
    static SharedObjectPool& instance();
};

} // namespace tcp