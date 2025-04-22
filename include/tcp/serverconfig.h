#pragma once

#include <cstddef>
#include <string>
namespace tcp
{
struct ServerConfig
{
    ServerConfig(const char* name = "TcpServer", const char* ip = "127.0.0.1", uint16_t port = 8080,
                 std::size_t maxConnections = 10000, std::size_t bufferSize = 1024, double timeout = 10.0);
    std::string name;
    std::string ip;
    uint16_t port;
    std::size_t maxConnections;
    std::size_t bufferSize;
    double timeout;
    const static ServerConfig defaultConfig;
};

inline const ServerConfig ServerConfig::defaultConfig = ServerConfig();

} // namespace tcp
