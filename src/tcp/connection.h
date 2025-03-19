#ifndef CONNECTION_H
#define CONNECTION_H
#include "channel.h"
#include <cstddef>
#include <functional>
#include <mutex>
#include <string>

#define BUFFER 1024
class EventLoop;
class Connection
{
    typedef std::function<void(int)> CloseCb;
    typedef std::function<void(Connection*, const std::string&)> MessageCb;

  public:
    enum State
    {
        Connected,
        DisConnected
    };
    // loop为nullptr表示客户端建立的
    Connection(int fd, EventLoop* loop);
    ~Connection();

    // 发送数据,线程安全
    void send(const char* data, size_t len);
    void send(const std::string& data);
    // 关闭连接
    void close();

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
    // 获取套接字
    int getSock() const
    {
        return fd_;
    };

  private:
    int fd_;
    EventLoop* loop_;
    Channel channel_;
    State state_;
    std::string readBuffer_;
    std::string writeBuffer_;
    size_t writeDataLeft_;
    std::mutex mutex_;

    std::function<void(int)> closeCb_;

    // 只有有数据才会触发，读事件没有数据表示客户端关闭连接
    std::function<void(Connection*, const std::string&)> messageCb_;
    // 读事件处理,客户端发送空数据不会触发读事件
    void handleRead();
    // 写事件处理
    void handleWrite();
    // 非阻塞写
    size_t writeNonBlock(const char* data, size_t len);

    void sendInLoop(const char* data, size_t len);
    void closeInLoop();
};
#endif