#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H
#include "ConnectionType.h"
#include "Socket.h"
#include "buffer.h"
#include "channel.h"
#include <any>
#include <atomic>
#include <cstddef>
#include <mutex>
#include <string>
#include <utility>

#define BUFFER 1024
class EventLoop;
class TcpConnection
{
  public:
    enum State
    {
        Connected,
        DisConnected
    };
    // loop为nullptr表示客户端建立的
    TcpConnection(Socket&& socket, EventLoop* loop, const InetAddress& localAddr, const InetAddress& peerAddr,
                  const std::string& name, const int id);
    ~TcpConnection();

    void start();
    // 发送数据,线程安全
    void send(const char* data, size_t len);
    void send(const std::string& data);
    // 关闭连接
    void close();
    void closeAfter(double delay);
    template <typename T> void setOnConnectionCb(T&& cb)
    {
        onConnectionCb_ = std::forward<T>(cb);
    };

    // 设置删除连接回调
    template <typename T> void setCloseCb(T&& cb)
    {
        closeCb_ = std::forward<T>(cb);
    };

    // 设置事件
    template <typename T> void setMessageCb(T&& cb)
    {
        messageCb_ = std::forward<T>(cb);
    };

    // 设置应用层的内容解析器
    template <typename T> void setContext(T&& context)
    {
        context_ = std::forward<T>(context);
    }

    std::any& getContext()
    {
        return context_;
    }
    int getId() const
    {
        return id_;
    }

    State getState() const
    {
        return state_;
    }
    const InetAddress& getLocalAddr() const
    {
        return localAddr_;
    }
    const InetAddress& getPeerAddr() const
    {
        return peerAddr_;
    }
    const std::string& getName() const
    {
        return name_;
    }

  private:
    Socket socket_;
    EventLoop* loop_;
    InetAddress localAddr_;
    InetAddress peerAddr_;
    std::string name_;
    int id_;
    Channel channel_;
    std::atomic<State> state_;
    Buffer readBuffer_;
    Buffer writeBuffer_;

    // 连接成功或连接关闭回调
    OnConnectionCb onConnectionCb_;
    // 关闭连接回调，由TcpServer调用
    CloseCb closeCb_;
    // 只有有数据才会触发，读事件没有数据表示客户端关闭连接
    MessageCb messageCb_;
    // 读事件处理,客户端发送空数据不会触发读事件
    std::any context_;
    void handleRead();
    // 写事件处理
    void handleWrite();
    // 非阻塞写
    int writeNonBlock(const char* data, size_t len);
    // 非阻塞读到缓冲区
    int readNonBlock();

    void sendInLoop(const char* data, size_t len);
    void closeInLoop();
    void startInLoop();
};
#endif