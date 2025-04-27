#pragma once
#include "acceptor.h"
#include "buffer.h"
#include "ioobject.h"
#include <any>
#include <atomic>
#include <cstddef>
#include <functional>
namespace tcp
{
class IoContext;
class InetAddress;
class Connection : public IoObject
{
  public:
    using ConnectTask = std::function<void(Connection*)>;
    using DisconnectTask = std::function<void(Connection*)>;
    using AfterReadTask = std::function<void(Connection*, const void*, std::size_t)>;
    enum State
    {
        Connected,
        DisConnected
    };
    // loop为nullptr表示客户端建立的
    Connection(int clientSocket, const InetAddress& clientAddr, IoContext* ioContext, Acceptor* acceptor);
    ~Connection();
    void start();
    void stop();
    void send(const char* data, size_t len);
    void send(const std::string& data);
    void send(std::string&& data);
    void onRead() override;
    void onWrite() override;
    std::size_t id() const;
    void setConnectTask(ConnectTask&& task);
    void setDisconnectTask(DisconnectTask&& task);
    void setAfterReadTask(AfterReadTask&& task);
    void close(double delay = 0.0);

  private:
    InetAddress addr_;
    const Acceptor* acceptor_;
    int id_;
    std::atomic<State> state_;
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

    void sendInThread(const char* data, size_t len);
    void closeInThread();
    void startInThread();
};
} // namespace tcp
