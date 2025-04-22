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
    // 停止Server
    void stop();
    // 设置连接建立、连接断开、接收到数据等回调函数，在start()之前设置，后续修改
    void setCallbacks(Callbacks callbacks);
    // 向指定连接发送信息
    void send(std::size_t connectionId, const void* data, std::size_t size);
    // 向所有连接发送信息
    void sendAll(const void* data, std::size_t size);
    // 断开指定连接
    void disconnect(std::size_t connectionId, double delay = 0.0);
    // 指定连接执行任务
    void doTask(std::size_t connectionId, Task task, double delay = 0.0, double interval = 0.0);
    // 向所有连接执行任务
    void doTaskAll(Task task, double delay = 0.0, double interval = 0.0);
    // 设置指定连接上下文
    void setContext(std::size_t connectionId, void* context);
    // 设置所有连接上下文
    void setContextAll(void* context);
    // 获取指定连接上下文`
    void* getContext(std::size_t connectionId) const;

  private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace tcp