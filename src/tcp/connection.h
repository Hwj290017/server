#ifndef CONNECTION_H
#define CONNECTION_H

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#define BUFFER 1024
class EventLoop;
class Socket;
class Channel;
class Connection
{
  public:
    enum State
    {
        Connected,
        DisConnected,
        WritedFull,
        WritePart
    };
    // loop为nullptr表示客户端建立的
    Connection(int fd, const EventLoop* loop = nullptr);
    ~Connection();

    // 读取数据
    std::string read();
    // 发送数据
    State send(const char* data, size_t len);
    State send(const std::string& data);
    // 关闭连接
    void close();

    // 设置删除连接回调
    void setCloseCb(std::function<void(int)> cb);
    // 设置事件
    void setReadCb(std::function<void(Connection*)> cb);
    // 获取状态
    State getState() const;
    // 获取套接字
    int getSock() const;

  private:
    int fd;
    const EventLoop* loop;
    std::unique_ptr<Channel> channel;
    State state;
    std::string readBuffer;
    std::string writeBuffer;
    size_t writeDateLeft;

    std::function<void(int)> closeCb;
    std::function<void(Connection*)> readCb;
    // 处理事件
    void handleEvent();
    // 非阻塞读
    void readNonBlock();
    // 非阻塞写
    void writeNonBlock();
};
#endif