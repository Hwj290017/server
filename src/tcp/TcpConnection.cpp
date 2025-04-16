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
#include <mutex>
#include <string>
#include <sys/types.h>
#include <unistd.h>

TcpConnection::TcpConnection(Socket&& socket, EventLoop* loop, const InetAddress& localAddr,
                             const InetAddress& peerAddr, const std::string& name, const int id)

    : socket_(std::move(socket)), loop_(loop), localAddr_(localAddr), peerAddr_(peerAddr), channel_(&socket_),
      state_(Connected), id_(id)
{
    channel_.setReadCb([this]() { handleRead(); });
    channel_.setWriteCb([this]() { handleWrite(); });
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

void TcpConnection::sendInLoop(const char* data, size_t len)
{
    if (len > 0)
    {
        auto writeNum = 0;
        if (writeBuffer_.size() == 0)
        {
            writeNum = writeNonBlock(data, len);
            Logger::logger << ("TcpConnection sendInLoop: " + std::to_string(id_) +
                               "\nWriteNum: " + std::to_string(writeNum)) +
                                  "\nWriteLeft: " + std::to_string(len - writeNum);
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

void TcpConnection::handleWrite()
{
    if (state_ == Connected)
    {
        auto writeNum = writeNonBlock(writeBuffer_.begin(), writeBuffer_.size());
        Logger::logger << ("TcpConnection handleWrite: " + std::to_string(id_) +
                           "\nWriteNum: " + std::to_string(writeNum));

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
        auto readNum = readNonBlock();
        Logger::logger << ("TcpConnection handleRead: " + std::to_string(id_) +
                           "\nReadNum: " + std::to_string(readNum));

        if (readNum <= 0)
        {
            close();
            return;
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
    // 执行函数时，处于Connected
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
                return len - dataLeft;
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
    // 全部写入
    return len;
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
                Logger::logger << std::string("hello");
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
    if (state_.exchange(DisConnected) == Connected)
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
    if (onConnectionCb_)
        onConnectionCb_(this);
    channel_.close();
    loop_->updateChannel(&channel_);
    // 在事件处理完后才关闭连接
    loop_->queueInLoop(std::bind(closeCb_, id_));
}
