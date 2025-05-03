#pragma once
#include "acceptor.h"
#include "buffer.h"
#include "sharedobject.h"
#include "tcp/connectionid.h"
#include <any>
#include <cstddef>

namespace tcp
{
class IoContext;
class InetAddress;

class AcceptorId;

class Connection : public SharedObject
{
  public:
    enum State
    {
        kConnected,
        kDisconnected,
    };
    // loop为nullptr表示客户端建立的
    Connection(int clientSocket, IoContext* ioContext, std::size_t id, const InetAddress& clientAddr);
    ~Connection();
    void start() override;
    void stop() override;
    void send(std::string&& data);
    void onRead() override;
    void onWrite() override;
    void setConnectTask(ConnectionId::ConnectTask&& task);
    void setDisconnectTask(ConnectionId::DisconnectTask&& task);
    void setAfterReadTask(ConnectionId::AfterReadTask&& task);

  private:
    InetAddress addr_;
    Buffer readBuffer_;
    Buffer writeBuffer_;

    // 连接成功或连接关闭回调
    ConnectionId::ConnectTask connectTask_;
    // 关闭连接回调，由TcpServer调用
    ConnectionId::DisconnectTask disconnectTask_;
    // 只有有数据才会触发，读事件没有数据表示客户端关闭连接
    ConnectionId::AfterReadTask afterReadTask_;

    // 读事件处理,客户端发送空数据不会触发读事件
    std::any context_;
    State state_;
    // 非阻塞写
    int writeNonBlock(const char* data, size_t len);
    // 非阻塞读到缓冲区
    int readNonBlock();
};
} // namespace tcp
