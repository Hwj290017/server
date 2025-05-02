#pragma once
#include "acceptor.h"
#include "buffer.h"
#include "sharedobject.h"
#include <any>
#include <atomic>
#include <cstddef>
#include <functional>
namespace tcp
{
class IoContext;
class InetAddress;

class ConnectionId;

class AcceptorId;

class Connection : public SharedObject
{
  public:
    using ConnectTask = std::function<void(const ConnectionId&)>;
    using DisconnectTask = std::function<void(const ConnectionId&)>;
    using AfterReadTask = std::function<void(const ConnectionId&, const void*, std::size_t)>;
    using StopTask = std::function<void()>;
    // loop为nullptr表示客户端建立的
    Connection(int clientSocket, IoContext* ioContext, std::size_t id, const AcceptorId& acceptorId,
               const InetAddress& clientAddr);
    ~Connection();
    void start();
    void stop();
    void send(const char* data, size_t len);
    void send(const std::string& data);
    void send(std::string&& data);
    void onRead() override;
    void onWrite() override;
    void setConnectTask(ConnectTask&& task);
    void setDisconnectTask(DisconnectTask&& task);
    void setAfterReadTask(AfterReadTask&& task);
    void setStopTask(StopTask&& task);

  private:
    InetAddress addr_;
    Buffer readBuffer_;
    Buffer writeBuffer_;

    // 连接成功或连接关闭回调
    ConnectTask connectTask_;
    // 关闭连接回调，由TcpServer调用
    DisconnectTask disconnectTask_;
    // 只有有数据才会触发，读事件没有数据表示客户端关闭连接
    AfterReadTask afterReadTask_;

    // 读事件处理,客户端发送空数据不会触发读事件
    std::any context_;
    // 非阻塞写
    int writeNonBlock(const char* data, size_t len);
    // 非阻塞读到缓冲区
    int readNonBlock();
};
} // namespace tcp
