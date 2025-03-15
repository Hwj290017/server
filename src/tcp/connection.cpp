#include "connection.h"
#include "channel.h"
#include "eventLoop.h"
#include "log.h"
#include <cerrno>
#include <cstddef>
#include <fcntl.h>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <sys/types.h>
#include <unistd.h>

Connection::Connection(int fd, EventLoop* loop) : fd_(fd), loop_(loop), state_(Connected)
{
    // 设置非阻塞IO
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
    // 创建channel
    channel_ = std::make_unique<Channel>(fd, loop);
    std::function<void()> readCb = std::bind(&Connection::handleRead, this);
    channel_->setReadCb(readCb);
    channel_->enableRead();
}

Connection::~Connection()
{
    channel_->close();
    if (fd_ != -1)
    {
        ::close(fd_);
        fd_ = -1;
    }
}

void Connection::handleRead()
{
    if (state_ == Connected)
    {
        char buf[BUFFER];
        while (true)
        { // 由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
            ssize_t readNum = ::read(fd_, buf, sizeof(buf));
            if (readNum == -1)
            { // 客户端正常中断、继续读取
                if (errno == EINTR)
                {
                    std::cout << "Read EINTR\n";
                    continue;
                }
                else if (errno == EAGAIN || errno == EWOULDBLOCK)
                { // 非阻塞IO，这个条件表示数据全部读取完毕
                    std::cout << "Read EAGAIN\n";
                    break;
                }
                else
                {
                    state_ = DisConnected;
                    close();
                    std::cout << "Read DisConnected\n";
                    return;
                }
            }
            else if (readNum == 0)
            { // EOF，客户端断开连接
                state_ = DisConnected;
                close();
                std::cout << "Read DisConnected\n";
                return;
            }
            // 读取成功
            readBuffer_.append(buf, readNum);
        }

        if (readBuffer_.length() > 0 && messageCb_)
        {
            messageCb_(this, readBuffer_);
            readBuffer_.clear();
        }
    }
}

void Connection::send(const char* data, size_t len)
{
    if (state_ == Connected)
    {
        size_t dataLeft = len;
        while (dataLeft > 0)
        {
            int writeNum = ::write(fd_, data + len - dataLeft, dataLeft);
            if (writeNum == -1)
            {
                if (errno == EINTR)
                // continue writing
                {
                    std::cout << "Write EINTR\n";
                    continue;
                }
                else if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    // 写缓冲区满
                    std::cout << "Write EAGAIN\n";
                    break;
                }
                else
                {
                    // Other error on client
                    state_ = DisConnected;
                    close();
                    std::cout << "Write DisConnected\n";
                    return;
                }
            }
            else if (writeNum == 0)
            {
                // Other error on client
                state_ = DisConnected;
                close();
                std::cout << "Write DisConnected\n";
                return;
            }
            std::cout << "Write: " << std::string(data + len - dataLeft, writeNum) << "\n";
            dataLeft -= writeNum;
        }
        // 未写完的数据放入写缓冲区
        if (dataLeft > 0)
        {
            writeBuffer_.append(data + len - dataLeft, dataLeft);
            writeDataLeft_ += dataLeft;
            channel_->enableWrite();
        }
    }
}

void Connection::send(const std::string& data)
{
    return send(data.c_str(), data.length());
}

void Connection::handleWrite()
{
    if (state_ == Connected)
    {
        int len = writeBuffer_.length();
        const char* buf = writeBuffer_.c_str();

        while (writeDataLeft_ > 0)
        {
            int writeNum = ::write(fd_, buf + len - writeDataLeft_, writeDataLeft_);
            if (writeNum == -1 && errno == EINTR)
            {
                // continue writing
                std::cout << "Write EINTR\n";
                continue;
            }
            if (writeNum == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
            {
                // 写缓冲区满
                std::cout << "Write EAGAIN\n";
                break;
            }

            if (writeNum == 0)
            {
                // Other error on client
                state_ = DisConnected;
                close();
                std::cout << "Write DisConnected\n";
                return;
            }
            std::cout << "Write: " << std::string(buf + len - writeDataLeft_, writeNum) << "\n";
            writeDataLeft_ -= writeNum;
        }

        if (writeDataLeft_ > 0)
        {
            channel_->enableWrite();
        }
        else
            writeBuffer_.clear();
    }
}

void Connection::setCloseCb(std::function<void(int)> cb)
{
    closeCb_ = std::move(cb);
}

void Connection::setMessageCb(std::function<void(Connection*, const std::string&)> cb)
{
    messageCb_ = std::move(cb);
}

void Connection::close()
{
    state_ = DisConnected;
    if (closeCb_)
        loop_->runInLoop(std::bind(closeCb_, fd_));
}

int Connection::getSock() const
{
    return fd_;
}

Connection::State Connection::getState() const
{
    return state_;
}