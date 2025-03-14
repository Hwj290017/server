#include "connection.h"
#include "channel.h"
#include "log.h"
#include <cerrno>
#include <cstddef>
#include <fcntl.h>
#include <functional>
#include <memory>
#include <string>
#include <sys/types.h>
#include <unistd.h>

Connection::Connection(int fd, const EventLoop* loop) : fd(fd), loop(loop), state(Connected)
{
    // 设置非阻塞IO
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
    // 创建channel
    channel = std::make_unique<Channel>(fd, loop);
    std::function<void()> readCb = std::bind(&Connection::handleEvent, this);
    channel->setReadCb(readCb);
    channel->enableRead();
}

Connection::~Connection()
{
    channel->close();
    if (fd != -1)
    {
        ::close(fd);
        fd = -1;
    }
}

std::string Connection::read()
{
    if (state == Connected)
    {
        readBuffer.clear();
        readNonBlock();
        return readBuffer;
    }
    close();
    return "";
}

Connection::State Connection::send(const char* data, size_t len)
{
    if (state == Connected)
    {
        writeBuffer.append(data, len);
        writeDateLeft += len;
        writeNonBlock();

        if (writeDateLeft == 0)
            return WritedFull;
        else
            return WritePart;
    }
    close();
    return DisConnected;
}

Connection::State Connection::send(const std::string& data)
{
    return send(data.c_str(), data.length());
}

void Connection::readNonBlock()
{
    char buf[BUFFER];
    while (true)
    { // 由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        ssize_t readNum = ::read(fd, buf, sizeof(buf));
        if (readNum == -1 && errno == EINTR)
        { // 客户端正常中断、继续读取
            logger << "EINTR\n";
            continue;
        }
        else if (readNum == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        { // 非阻塞IO，这个条件表示数据全部读取完毕
            logger << "EAGAIN\n";
            break;
        }
        else if (readNum == 0)
        { // EOF，客户端断开连接
            state = DisConnected;
            logger << "DisConnected\n";
            close();
            return;
        }
        // 读取成功
        readBuffer.append(buf, readNum);
        logger << "read: " << readBuffer << "\n";
    }
}

void Connection::writeNonBlock()
{
    int dataSize = writeBuffer.length();
    const char* buf = writeBuffer.c_str();
    while (writeDateLeft > 0)
    {
        int writeNum = ::write(fd, buf + dataSize - writeDateLeft, writeDateLeft);
        if (writeNum == -1 && errno == EINTR)
        {
            // continue writing
            logger << "EINTR\n";
            continue;
        }
        if (writeNum == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
        {
            // 写缓冲区满
            logger << "EAGAIN\n";
            break;
        }
        if (writeNum == 0)
        {
            // Other error on client
            state = DisConnected;
            logger << "DisConnected\n";
            close();
            return;
        }
        writeDateLeft -= writeNum;
    }

    if (writeDateLeft > 0)
    {
        channel->enableWrite();
    }
    else
    {
        writeBuffer.clear();
    }
}

void Connection::setCloseCb(std::function<void(int)> cb)
{
    closeCb = std::move(cb);
}

void Connection::setReadCb(std::function<void(Connection*)> cb)
{
    readCb = std::move(cb);
}
void Connection::handleEvent()
{
    readCb(this);
}

void Connection::close()
{
    closeCb(fd);
}

int Connection::getSock() const
{
    return fd;
}

Connection::State Connection::getState() const
{
    return state;
}