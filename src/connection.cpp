#include "connection.h"
#include "EventLoop.h"
#include "channel.h"
#include <cerrno>
#include <fcntl.h>
#include <functional>
#include <memory>
#include <string>
#include <sys/types.h>
#include <unistd.h>

Connection::Connection(int fd, const EventLoop* loop) : fd(fd), loop(loop), state(Connected)
{
    if (loop)
    { // 服务端
        // 客户端非堵塞
        fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
        channel = std::make_unique<Channel>(fd, loop);
    }
}

Connection::~Connection()
{
    if (fd >= 0)
    {
        ::close(fd);
        fd = -1;
    }
}

void Connection::setDeleteConnectionCb(std::function<void(int)> cb)
{
    deleteConnectionCb = std::move(cb);
}

void Connection::setOnConnectionCb(std::function<void(Connection*)> cb)
{
    onConnectionCb = std::move(cb);
    channel->setCallback(std::bind(onConnectionCb, this));
    channel->enableReading();
}

void Connection::read()
{
    if (state == Connected)
    {
        readBuffer.clear();
        if (loop)
            readNonBlock();
        else
            readBlock();
    }
}

void Connection::write()
{
    if (state == Connected)
    {
        if (loop)
            writeNonBlock();
        else
            writeBlock();
        writeBuffer.clear();
    }
}
void Connection::readNonBlock()
{
    char buf[BUFFER];
    while (true)
    { // 由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        ssize_t readNum = ::read(fd, buf, sizeof(buf));
        if (readNum == -1 && errno == EINTR)
        { // 客户端正常中断、继续读取
            continue;
        }
        else if (readNum == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        { // 非阻塞IO，这个条件表示数据全部读取完毕
            break;
        }
        else if (readNum == 0)
        { // EOF，客户端断开连接
            state = Closed;
            break;
        }
        // 读取成功
        readBuffer.append(buf, readNum);
    }
}

void Connection::writeNonBlock()
{
    int dataSize = writeBuffer.length();
    int dataLeft = dataSize;
    const char* buf = writeBuffer.c_str();
    while (dataLeft > 0)
    {
        int writeNum = ::write(fd, buf + dataSize - dataLeft, dataLeft);
        if (writeNum == -1 && errno == EINTR)
        {
            // continue writing
            continue;
        }
        if (writeNum == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
        {
            // 写缓冲区满
            break;
        }
        if (writeNum == 0)
        {
            // Other error on client
            state = State::Closed;
            break;
        }
        dataLeft -= writeNum;
    }
}

void Connection::readBlock()
{

    char buf[BUFFER];
    while (true)
    {
        // 阻塞套接字读取数据
        ssize_t readNum = ::read(fd, buf, sizeof(buf));
        if (readNum <= 0)
        {
            if (errno == EINTR)
                // continue reading
                continue;
            else
            {
                // Other error on client
                state = State::Closed;
                break;
            };
        }
        readBuffer.append(buf, readNum);
    }
}

void Connection::writeBlock()
{
    int dataSize = writeBuffer.length();
    int dataLeft = dataSize;
    const char* buf = writeBuffer.c_str();
    while (dataLeft > 0)
    {
        int writeNum = ::write(fd, buf + dataSize - dataLeft, dataLeft);
        if (writeNum <= 0)
        {
            if (errno == EINTR)
                // continue writing
                continue;
            else
            {
                state = State::Closed;
                break;
            }
        }
        // 写入成功
        dataLeft -= writeNum;
    }
}
std::string Connection::getReadBuffer()
{
    return readBuffer;
}

void Connection::setWriteBuffer(const std::string& data)
{
    writeBuffer.append(data);
}
void Connection::close()
{
    state = Closed;
    deleteConnectionCb(fd);
}
int Connection::getSock() const
{
    return fd;
}

Connection::State Connection::getState() const
{
    return state;
}