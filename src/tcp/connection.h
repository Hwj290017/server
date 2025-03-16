#ifndef CONNECTION_H
#define CONNECTION_H

#include <cstddef>
#include <functional>
#include <memory>
#include <string>

#define BUFFER 1024
class EventLoop;
class Channel;
class Connection
{
  public:
    enum State
    {
        Connected,
        DisConnected
    };
    // loop为nullptr表示客户端建立的
    Connection(int fd, EventLoop* loop = nullptr);
    ~Connection();

    // 发送数据
    void send(const char* data, size_t len);
    void send(const std::string& data);
    // 关闭连接
    void close();

    // 设置删除连接回调
    void setCloseCb(const std::function<void(int)>& cb);
    // 设置事件
    void setMessageCb(const std::function<void(Connection*, const std::string&)>& cb);
    // 获取状态
    State getState() const;
    // 获取套接字
    int getSock() const;

  private:
    int fd_;
    EventLoop* loop_;
    std::unique_ptr<Channel> channel_;
    State state_;
    std::string readBuffer_;
    std::string writeBuffer_;
    size_t writeDataLeft_;

    std::function<void(int)> closeCb_;
    std::function<void(Connection*, const std::string&)> messageCb_;
    // 读事件处理
    void handleRead();
    // 写事件处理
    void handleWrite();
    // 非阻塞写
    size_t writeNonBlock(const char* data, size_t len);
};
#endif