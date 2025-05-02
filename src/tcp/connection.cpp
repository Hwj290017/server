#include "connection.h"
#include "acceptorId.h"
#include "connectionid.h"
#include "iocontext.h"
#include "log/log.h"
#include "object.h"
#include "sharedobject.h"
#include <cassert>
#include <cerrno>
#include <cstddef>
#include <fcntl.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>
namespace tcp
{
Connection::Connection(int clientSocket, IoContext* ioContext, std::size_t id, const AcceptorId& acceptorId,
                       const InetAddress& clientAddr)

    : SharedObject(clientSocket, ioContext, id, acceptorId), addr_(clientAddr)
{
}

Connection::~Connection()
{
}

void Connection::start()
{
    ioContext_->enableRead(this);
}

void Connection::stop()
{
    ioContext_->disableRead(this);
}

void Connection::send(std::string&& data)
{
}
// void Connection::sendInLoop(const char* data, size_t len)
// {
//     if (len > 0)
//     {
//         auto writeNum = 0;
//         if (writeBuffer_.size() == 0)
//         {
//             writeNum = writeNonBlock(data, len);
//             Logger::logger << ("TcpConnection sendInLoop: " + std::to_string(id_) +
//                                "\nWriteNum: " + std::to_string(writeNum)) +
//                                   "\nWriteLeft: " + std::to_string(len - writeNum);
//             if (writeNum < 0)
//             {
//                 close();
//                 return;
//             }
//         }

//         if (writeNum < len)
//         {
//             // 未写完的数据放入写缓冲区
//             writeBuffer_.append(data + writeNum, len - writeNum);
//             channel_.enableWrite();
//             loop_->updateChannel(&channel_);
//         }
//     }
// }

void Connection::onWrite()
{
    Logger::logger << ("TcpConnection handleWrite: " + id().toString() + "\nWriteNum: " + std::to_string(writeNum));

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

void Connection::onRead()
{

    Logger::logger << ("TcpConnection handleRead: " + id_.toString());

    auto success = readBuffer_.readSocket(fd_);
    if (success)
    {
        if (afterReadTask_)
            afterReadTask_(ConnectionId(id_, pool_), readBuffer_.begin(), readBuffer_.size());
    }
    else
    {
        stop();
    }
}
}

// int TcpConnection::writeNonBlock(const char* data, size_t len)
// {
//     // 执行函数时，处于Connected
//     auto dataLeft = len;
//     while (dataLeft > 0)
//     {
//         auto writeNum = socket_.write(data + len - dataLeft, dataLeft);
//         if (writeNum < 0)
//         {
//             if (errno == EINTR)
//             // continue writing
//             {
//                 continue;
//             }
//             else if (errno == EAGAIN || errno == EWOULDBLOCK)
//             {
//                 // 写缓冲区满
//                 return len - dataLeft;
//             }
//             else
//             {
//                 // Other error on client
//                 return -1;
//             }
//         }
//         else if (writeNum == 0)
//         {
//             // Other error on client
//             return -1;
//         }
//         dataLeft -= writeNum;
//     }
//     // 全部写入
//     return len;
// }
// int TcpConnection::readNonBlock()
// {
//     auto readNumSum = 0;
//     while (true)
//     {
//         int readNum = readBuffer_.readSocket(socket_);
//         if (readNum <= 0)
//         { // 客户端正常中断、继续读取
//             if (errno == EINTR)
//             {
//                 continue;
//             }
//             else if (errno == EAGAIN || errno == EWOULDBLOCK)
//             { // 非阻塞IO，这个条件表示数据全部读取完毕
//                 Logger::logger << std::string("hello");
//                 return readNumSum;
//             }
//             else
//             {
//                 // client断开连接
//                 return -1;
//             }
//         }
//         else
//         {
//             readNumSum += readNum;
//         }
//     }
// }
// 暂时没处理延时关闭

// void TcpConnection::closeAfter(double delay)
// {
//     loop_->runAfter(std::bind(&TcpConnection::close, this), delay);
// }

// void TcpConnection::closeInLoop()
// {
//     assert(state_ == DisConnected);
//     if (onConnectionCb_)
//         onConnectionCb_(this);
//     channel_.close();
//     loop_->updateChannel(&channel_);
//     // 在事件处理完后才关闭连接
//     loop_->queueInLoop(std::bind(closeCb_, id_));
// }

} // namespace tcp
