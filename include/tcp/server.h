#pragma once

#include "tcp/iocontextid.h"
#include <memory>
namespace tcp
{
struct ServerConfig;
class Server
{
  public:
    Server(const ServerConfig& config);
    ~Server();
    // 开启Server
    void start();
    // get a  random IoContext
    IoContextId getIoContextId();

  private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace tcp