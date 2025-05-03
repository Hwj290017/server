#pragma once

#include "iocontext.h"
#include "tcp/acceptor.h"
#include "tcp/connection.h"
#include "tcp/connector.h"
#include "tcp/inetaddress.h"
#include "tcp/object.h"
#include <cstddef>
#include <memory>
#include <mutex>
#include <vector>
namespace tcp
{
class Manager
{
  public:
    using IoContextPtr = std::unique_ptr<IoContext>;
    static constexpr std::size_t kInitialIoContextNum = 8;

    void start();
    void newAcceptor(const InetAddress& listenAddr, const Acceptor::Tasks& tasks, const Object* parent = nullptr);
    void newConnection(int clientfd, const InetAddress& serverAddr, const Connection::Tasks& tasks,
                       const Object* parent = nullptr);
    void newConnector(const InetAddress& serverAddr, const Connector::Tasks& tasks, const Object* parent = nullptr);
    void isAlive(Object* object);
    void getParent(Object* object);
    static Manager& instance();

    // private:
    //   Manager(std::size_t ioContextNum = kInitialIoContextNum);
    //   ~Manager();
    //   std::size_t ioContextNum_;
    //   std::vector<IoContextPtr> ioContexts_;
    //   std::mutex mutex_;
    //   std::size_t currentIndex_;
};
} // namespace tcp
