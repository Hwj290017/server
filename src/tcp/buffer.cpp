#include "buffer.h"
#include "Socket.h"
#include <algorithm>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <unistd.h>

void Buffer::append(const char* data, int length)
{
    ableAppend(length);
    std::copy(data, data + length, data_.data() + endIndex_);
    endIndex_ += length;
}

Buffer& Buffer::operator+=(const char* data)
{
    append(data, strlen(data));
    return *this;
}

Buffer& Buffer::operator+=(const std::string& data)
{
    append(data.data(), data.size());
    return *this;
}

// void Buffer::prepend(const char* data, int length)
// {
//     ablePrepend(length);
//     std::copy(data, data + length, data_.data() + beginIndex_ - length);
//     beginIndex_ -= length;
// }

void Buffer::ableAppend(int length)
{

    if (endIndex_ + length <= data_.size())
        return;
    int space = beginIndex_ + data_.size() - endIndex_;
    char* str = data_.data();

    if (space >= length + pretendedLength)
    {
        std::copy(str + beginIndex_, str + endIndex_, str + pretendedLength);
        endIndex_ = endIndex_ + pretendedLength - beginIndex_;
        beginIndex_ = pretendedLength;
    }
    else
    {
        data_.resize(endIndex_ + length);
    }
}

const char* Buffer::findCRLF(const char* start) const
{
    const char* str = data_.data();
    return std::search(start, str + endIndex_, CRLF, CRLF + 2);
}
// void Buffer::ablePrepend(int length)
// {
//     int space = beginIndex_ + data_.size() - endIndex_;
//     const char* str = data_.data();
//     if (space >= length + pretendedLength)
//     {
//         std::copy(str + beginIndex_, str + endIndex_, str + pretendedLength);
//         endIndex_ = beginIndex_ + pretendedLength - beginIndex_;
//         beginIndex_ = pretendedLength;
//     }
//     data_.resize(endIndex_ + length);
//     }

int Buffer::readSocket(const Socket& socket)
{
    // 由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
    ableAppend(initialLength);
    int readNum = socket.read(data_.data() + endIndex_, data_.size() - endIndex_);
    if (readNum > 0)
        endIndex_ += readNum;

    return readNum;
}
