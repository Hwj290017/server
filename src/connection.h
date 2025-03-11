#ifndef CONNECTION_H
#define CONNECTION_H
#include <functional>
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
        Invalid = 1,
        Handshaking,
        Connected,
        Closed,
        Failed,
    };
    // loop为nullptr表示客户端建立的
    Connection(Socket* sock, EventLoop* loop = nullptr);
    ~Connection();
    // 从套接字读到缓冲区
    void read();
    // 从缓冲区写到套接字
    void write();
    // 读取缓冲区
    std::string getReadBuffer();
    // 写入缓冲区
    void setWriteBuffer(const std::string& data);
    // 设置删除连接回调
    void setDeleteConnectionCb(std::function<void(Socket*)> cb);
    // 设置事件
    void setOnConnectionCb(std::function<void(Connection*)> cb);
    // 获取状态
    State getState() const;
    // 获取套接字
    const Socket* getSock() const;
    // 关闭连接
    void close();

  private:
    EventLoop* loop;
    Socket* sock;
    Channel* channel;
    State state;
    std::string readBuffer;
    std::string writeBuffer;

    std::function<void(Socket*)> deleteConnectionCb;
    std::function<void(Connection*)> onConnectionCb;

    // 非阻塞读
    void readNonBlock();
    // 非阻塞写
    void writeNonBlock();
    // 阻塞读
    void readBlock();
    // 阻塞写
    void writeBlock();
};
#endif