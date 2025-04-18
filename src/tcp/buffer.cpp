#include "buffer.h"
#include "Socket.h"
#include <algorithm>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <vector>

void Buffer::append(const char* data, int length)
{
    ableAppend(length);
    std::copy(data, data + length, data_.data() + endIndex_);
    endIndex_ += length;
}

void Buffer::prepend(const char* data, int length)
{
    ablePrepend(length);
    std::copy(data, data + length, data_.data() + beginIndex_ - length);
    beginIndex_ -= length;
}

const char* Buffer::findCRLF(const char* start) const
{
    const char* str = data_.data();
    return std::search(start, str + endIndex_, CRLF, CRLF + 2);
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

void Buffer::ableAppend(int length)
{

    if (endIndex_ + length <= data_.size())
        return;
    int space = data_.size() - size();
    char* str = data_.data();

    if (space >= length + pretendedLength)
    {
        std::copy(str + beginIndex_, str + endIndex_, str + pretendedLength);
        endIndex_ = pretendedLength + size();
        beginIndex_ = pretendedLength;
    }
    else
    {
        data_.resize(endIndex_ + length);
    }
}

void Buffer::ablePrepend(int length)
{
    if (beginIndex_ >= length)
        return;
    int space = data_.size() - size();
    char* str = data_.data();
    if (space >= length + pretendedLength)
    {
        std::copy_backward(str + beginIndex_, str + endIndex_, str + length + pretendedLength + size());
        endIndex_ = pretendedLength + length + size();
        beginIndex_ = pretendedLength + length;
    }
    else
    {
        // 新建一个刚好大小的buffer
        std::vector<char> newData(length + size() + pretendedLength);
        std::copy(str + beginIndex_, str + endIndex_, newData.data() + length + pretendedLength);
        data_.swap(newData);
        endIndex_ = length + size() + pretendedLength;
        beginIndex_ = length + pretendedLength;
    }
}

int Buffer::readSocket(const Socket& socket)
{
    // 由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
    ableAppend(initialLength);
    int readNum = socket.read(data_.data() + endIndex_, data_.size() - endIndex_);

    if (readNum > 0)
        endIndex_ += readNum;

    return readNum;
}
