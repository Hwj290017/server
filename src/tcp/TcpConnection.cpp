#include "TcpConnection.h"
#include "Socket.h"
#include "channel.h"
#include "eventLoop.h"
#include "timer.h"
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

TcpConnection::TcpConnection(Socket&& socket, EventLoop* loop, const InetAddress& localAddr,
                             const InetAddress& peerAddr, const std::string& name, const int id)

    : socket_(std::move(socket)), loop_(loop), channel_(&socket_), state_(Connected), id_(id)
{
    socket_.nonBlocking();
    channel_.setReadCb(std::bind(&TcpConnection::handleRead, this));
    channel_.enableRead();
    channel_.enableEt();
    std::cout << "TcpConnection" << id << " created" << std::endl;
}

TcpConnection::~TcpConnection()
{
}

void TcpConnection::start()
{
    loop_->runInLoop(std::bind(&TcpConnection::startInLoop, this));
}

void TcpConnection::startInLoop()
{

    loop_->updateChannel(&channel_);
    if (onConnectionCb_)
        onConnectionCb_(this);
    std::cout << "TcpConnection" << id_ << " realCreated" << std::endl;
}

void TcpConnection::handleRead()
{
    if (state_ == Connected)
    {
        while (true)
        {
            int readNum = readBuffer_.readSocket(socket_);
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
        }
        if (readBuffer_.size() > 0 && messageCb_)
        {
            messageCb_(this, readBuffer_, TimeSpec({10}));
            readBuffer_.clear();
        }
    }
}

int TcpConnection::writeNonBlock(const char* data, size_t len)
{
    if (state_ == Connected)
    {
        size_t dataLeft = len;
        while (dataLeft > 0)
        {
            int writeNum = socket_.write(data + len - dataLeft, dataLeft);
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
                    return 0;
                }
                else
                {
                    // Other error on client
                    return -1;
                }
            }
            else if (writeNum == 0)
            {
                // Other error on client
                return -1;
            }
            dataLeft -= writeNum;
        }
        return len - dataLeft;
    }
    return -1;
}

void TcpConnection::send(const char* data, size_t len)
{
    if (state_ == Connected)
    {
        loop_->runInLoop(std::bind(&TcpConnection::sendInLoop, this, data, len));
    }
}
void TcpConnection::send(const std::string& data)
{
    return send(data.c_str(), data.length());
}

void TcpConnection::handleWrite()
{
    if (state_ == Connected)
    {
        int writeNum = writeNonBlock(writeBuffer_.begin(), writeBuffer_.size());
        if (writeNum < 0)
        {
            close();
            return;
        }

        if (writeBuffer_.size() > 0)
        {
            // 未写完
            channel_.enableWrite();
            loop_->updateChannel(&channel_);
        }
        else
            writeBuffer_.clear();
    }
}

void TcpConnection::close()
{
    // 多个线程可能同时调用close
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
        loop_->runInLoop(std::bind(&TcpConnection::closeInLoop, this));
    }
}

void TcpConnection::closeInLoop()
{
    assert(state_ == DisConnected);
    channel_.quit();
    loop_->updateChannel(&channel_);
    // 如果是别的线程调用close，还要等一个周期才会执行closeCb_
    loop_->queueInLoop(std::bind(closeCb_, id_));
}

void TcpConnection::sendInLoop(const char* data, size_t len)
{
    if (state_ == Connected)
    {
        if (len > 0)
        {
            size_t writeNum = 0;
            if (writeBuffer_.size() == 0)
            {
                writeNum = writeNonBlock(data, len);
                if (writeNum < 0)
                {
                    close();
                    return;
                }
            }

            if (writeNum < len)
            {
                // 未写完的数据放入写缓冲区
                writeBuffer_.append(data + writeNum, len - writeNum);
                channel_.enableWrite();
                loop_->updateChannel(&channel_);
            }
        }
    }
}
