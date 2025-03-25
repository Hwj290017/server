#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H
#include "ConnectionType.h"
#include "Socket.h"
#include "buffer.h"
#include "channel.h"
#include "timer.h"
#include <any>
#include <cstddef>
#include <functional>
#include <mutex>
#include <string>

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

    void setOnConnectionCb(const OnConnectionCb& cb)
    {
        onConnectionCb_ = cb;
    };
    void setOnConnectionCb(OnConnectionCb&& cb)
    {
        onConnectionCb_ = std::move(cb);
    };
    // 设置删除连接回调
    void setCloseCb(const CloseCb& cb)
    {
        closeCb_ = cb;
    };
    void setCloseCb(CloseCb&& cb)
    {
        closeCb_ = std::move(cb);
    };
    // 设置事件
    void setMessageCb(const MessageCb& cb)
    {
        messageCb_ = cb;
    };
    void setMessageCb(MessageCb&& cb)
    {
        messageCb_ = std::move(cb);
    }
    // 获取状态
    State getState() const
    {
        return state_;
    };

    int getFd()
    {
        return socket_.fd();
    }

    void setContext(const std::any& context)
    {
        context_ = context;
    }

    void setContext(std::any&& context)
    {
        context_ = std::move(context);
    }
    std::any& getContext()
    {
        return context_;
    }

  private:
    Socket socket_;
    EventLoop* loop_;
    InetAddress localAddr_;
    InetAddress peerAddr_;
    std::string name_;
    int id_;
    Channel channel_;
    State state_;
    Buffer readBuffer_;
    Buffer writeBuffer_;
    std::mutex mutex_;

    // 连接成功回调
    OnConnectionCb onConnectionCb_;
    // 关闭连接回调
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

    void sendInLoop(const char* data, size_t len);
    void closeInLoop();
    void startInLoop();
};
#endif