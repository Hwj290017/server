#pragma once

#include <cstddef>
#include <functional>
namespace tcp
{
class Server;
using Task = std::function<void()>;
// 这些回调都需要通过server的接口
class Callbacks
{
  public:
    using ConnectCallback = std::function<void(std::size_t connectionId)>;
    using DisconnectCallback = std::function<void(std::size_t connectionId)>;
    using DataCallback = std::function<void(std::size_t connectionId, const void* data, std::size_t length)>;

    Callbacks() = default;
    ~Callbacks() = default;
    ConnectCallback connectCallback;
    DisconnectCallback disconnectCallback;
    DataCallback dataCallback;
};

} // namespace tcp