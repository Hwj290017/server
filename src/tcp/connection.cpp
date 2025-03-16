#include "connection.h"
#include "channel.h"
#include "eventLoop.h"
#include "util.h"
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
    channel_ = std::make_unique<Channel>(fd);
    std::function<void()> readCb = std::bind(&Connection::handleRead, this);
    channel_->setReadCb(readCb);
    channel_->enableRead();
    channel_->enableEt();
    loop_->addChannel(channel_.get());
}

Connection::~Connection()
{
    loop_->removeChannel(channel_.get());
    errif(fd_ < 0, "Invalid connection fd");
    ::close(fd_);
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
                    close();
                    std::cout << "Read DisConnected\n";
                    return;
                }
            }
            else if (readNum == 0)
            { // EOF，客户端断开连接
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

size_t Connection::writeNonBlock(const char* data, size_t len)
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
                    close();
                    std::cout << "Write DisConnected\n";
                    return -1;
                }
            }
            else if (writeNum == 0)
            {
                // Other error on client
                close();
                std::cout << "Write DisConnected\n";
                return -1;
            }
            dataLeft -= writeNum;
        }
        return len - dataLeft;
    }
    return -1;
}

void Connection::send(const char* data, size_t len)
{
    if (state_ == Connected)
    {
        size_t writeNum = writeNonBlock(data, len);
        if (writeNum < 0)
            return;
        if (writeNum < len)
        {
            // 未写完的数据放入写缓冲区
            writeBuffer_.append(data + writeNum, len - writeNum);
            writeDataLeft_ += (len - writeNum);
            channel_->enableWrite();
            loop_->updateChannel(channel_.get());
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
        const char* data = writeBuffer_.c_str();

        size_t writeNum = writeNonBlock(data + len - writeDataLeft_, writeDataLeft_);
        if (writeNum < 0)
            return;
        writeDataLeft_ -= writeNum;
        if (writeDataLeft_ > 0)
        {
            // 未写完
            channel_->enableWrite();
            loop_->updateChannel(channel_.get());
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
    if (state_ == Connected && closeCb_)
    {
        state_ = DisConnected;
        loop_->runInLoop(std::bind(closeCb_, fd_));
    }
}

int Connection::getSock() const
{
    return fd_;
}

Connection::State Connection::getState() const
{
    return state_;
}