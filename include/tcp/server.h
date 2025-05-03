#pragma once

#include "tcp/callbacks.h"
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

  private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace tcp