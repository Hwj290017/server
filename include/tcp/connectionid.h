#pragma once
#include "tcp/acceptorId.h"
#include <any>
#include <cstddef>
#include <functional>
namespace tcp
{
class ConnectionId
{
  public:
    using AferConnectTask = std::function<void(ConnectionId)>;
    using BeforeDisconnectTask = std::function<void(ConnectionId)>;
    using AfterReadTask = std::function<void(ConnectionId, const void*, std::size_t)>;
    using ConnectionTask = std::function<void(ConnectionId)>;

    void start();
    // 在连接时执行
    void aferConnect(AferConnectTask task);
    // 在断开连接时执行
    void beforeDisconnect(BeforeDisconnectTask task);
    // 在接收到数据后执行
    void afterRead(AfterReadTask task);
    void stop();
    // 发送数据
    void send(const void* data, std::size_t size);
    // 执行
    void doTask(ConnectionTask task, double delay = 0.0, double interval = 0.0);
    // 设置上下文
    void setContext(std::any context);
    // 获取上下文
    std::any& getContext();
    // 获取此连接的acceptorid
    AcceptorId getAcceptorId();
};
} // namespace tcp
