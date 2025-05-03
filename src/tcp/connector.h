#pragma once
#include "buffer.h"
#include "iocontext.h"
#include "sharedobject.h"
#include "tcp/connectorid.h"
#include "tcp/inetaddress.h"
namespace tcp
{

class Connector : public SharedObject
{
  public:
    enum class State
    {
        kConnected,
        kDisconnected
    };
    Connector(IoContext* ioContext, std::size_t id, const InetAddress& serverAddr);
    ~Connector();
    void start() override;
    void stop() override;
    void send(std::string&& data);
    void onRead() override;
    void onWrite() override;
    void setAfterReadTask(ConnectorId::AfterReadTask&& task);
    void setConnectTask(ConnectorId::ConnectTask&& task);
    void setDisconnectTask(ConnectorId::DisconnectTask&& task);

  private:
    InetAddress serverAddr_;
    Buffer readBuffer_;
    Buffer writeBuffer_;

    // 连接成功或连接关闭回调
    ConnectorId::ConnectTask connectTask_;
    // 关闭连接回调，由TcpServer调用
    ConnectorId::DisconnectTask disconnectTask_;
    // 只有有数据才会触发，读事件没有数据表示客户端关闭连接
    ConnectorId::AfterReadTask afterReadTask_;

    // 读事件处理,客户端发送空数据不会触发读事件
    std::any context_;
    State state_;
    // 非阻塞写
};
} // namespace tcp
