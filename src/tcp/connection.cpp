#include "connection.h"
#include "channel.h"
#include "eventLoop.h"
#include "util.h"
#include <cassert>
#include <cerrno>
#include <cstddef>
#include <fcntl.h>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <sys/types.h>
#include <unistd.h>

Connection::Connection(int fd, EventLoop* loop) : fd_(fd), loop_(loop), channel_(fd_), state_(Connected)
{
    // 设置非阻塞IO
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);

    channel_.setReadCb(std::bind(&Connection::handleRead, this));
    channel_.enableRead();
    channel_.enableEt();
    channel_.start();
    loop_->updateChannel(&channel_);
}

Connection::~Connection()
{
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
            if (readNum < 0)
            { // 客户端正常中断、继续读取
                if (errno == EINTR)
                {
                    continue;
                }
                else if (errno == EAGAIN || errno == EWOULDBLOCK)
                { // 非阻塞IO，这个条件表示数据全部读取完毕
                    break;
                }
                else
                {
                    close();
                    break;
                }
            }
            else if (readNum == 0)
            { // EOF，客户端断开连接
                close();
                break;
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
            if (writeNum < 0)
            {
                if (errno == EINTR)
                // continue writing
                {
                    continue;
                }
                else if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    // 写缓冲区满
                    break;
                }
                else
                {
                    // Other error on client
                    close();
                    break;
                }
            }
            else if (writeNum == 0)
            {
                // Other error on client
                close();
                break;
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
        loop_->runInLoop(std::bind(&Connection::sendInLoop, this, data, len));
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
            channel_.enableWrite();
            loop_->updateChannel(&channel_);
        }
        else
            writeBuffer_.clear();
    }
}

void Connection::close()
{
    // 多个线程同时调用close
    bool flag = false;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (state_ == Connected)
        {
            state_ = DisConnected;
            flag = true;
        }
    }

    if (flag)
    {
        loop_->runInLoop(std::bind(&Connection::closeInLoop, this));
    }
}

void Connection::closeInLoop()
{
    assert(state_ == DisConnected);
    channel_.quit();
    loop_->updateChannel(&channel_);
    // 如果是别的线程调用close，还要等一个周期才会执行closeCb_
    loop_->queueInLoop(std::bind(closeCb_, fd_));
}

void Connection::sendInLoop(const char* data, size_t len)
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
            channel_.enableWrite();
            loop_->updateChannel(&channel_);
        }
    }
}
