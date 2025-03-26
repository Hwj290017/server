#include "TcpConnection.h"
#include "Socket.h"
#include "channel.h"
#include "eventLoop.h"
#include "log.h"
#include "timer.h"
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
    channel_.start();
    loop_->updateChannel(&channel_);
    if (onConnectionCb_)
        onConnectionCb_(this);
    Logger::logger << ("TcpConnection realCreated: " + std::to_string(id_));
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
        Logger::logger << ("TcpConnection handleWrite: " + std::to_string(id_));
        auto writeNum = writeNonBlock(writeBuffer_.begin(), writeBuffer_.size());
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

void TcpConnection::handleRead()
{
    if (state_ == Connected)
    {
        Logger::logger << ("TcpConnection handleRead: " + std::to_string(id_));
        auto readNum = readNonBlock();
        if (readNum < 0)
        {
            close();
        }

        else if (messageCb_)
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
        auto dataLeft = len;
        while (dataLeft > 0)
        {
            auto writeNum = socket_.write(data + len - dataLeft, dataLeft);
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
int TcpConnection::readNonBlock()
{
    auto readNumSum = 0;
    while (true)
    {
        int readNum = readBuffer_.readSocket(socket_);
        if (readNum <= 0)
        { // 客户端正常中断、继续读取
            if (errno == EINTR)
            {
                continue;
            }
            else if (errno == EAGAIN || errno == EWOULDBLOCK)
            { // 非阻塞IO，这个条件表示数据全部读取完毕
                return readNumSum;
            }
            else
            {
                // client断开连接
                return -1;
            }
        }
        else
        {
            readNumSum += readNum;
        }
    }
}

void TcpConnection::close()
{
    // 多个线程可能同时调用close
    auto flag = false;
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

void TcpConnection::closeAfter(double delay)
{
    loop_->runAfter(std::bind(&TcpConnection::close, this), delay);
}

void TcpConnection::closeInLoop()
{
    assert(state_ == DisConnected);
    channel_.close();
    loop_->updateChannel(&channel_);
    // 在事件处理完后才关闭连接
    loop_->queueInLoop(std::bind(closeCb_, id_));
}

void TcpConnection::sendInLoop(const char* data, size_t len)
{
    if (state_ == Connected)
    {
        Logger::logger << ("TcpConnection sendInLoop: " + std::to_string(id_));
        if (len > 0)
        {
            auto writeNum = 0;
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
