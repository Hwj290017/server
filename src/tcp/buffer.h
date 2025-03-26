#ifndef BUFFER_H
#define BUFFER_H
#include <string>
#include <vector>
class Socket;
class Buffer
{
  public:
    const static int initialLength = 1024;
    const static int pretendedLength = 8;
    constexpr static char CRLF[3] = "\r\n";
    Buffer() : data_(initialLength), beginIndex_(pretendedLength), endIndex_(pretendedLength)
    {
    }

    void append(const char* data, int length);

    void prepend(const char* data, int length);
    const char* findCRLF(const char* start) const;

    Buffer& operator+=(const char*);
    Buffer& operator+=(const std::string&);
    void clear()
    {
        beginIndex_ = pretendedLength;
        endIndex_ = pretendedLength;
    }
    int size() const
    {
        return endIndex_ - beginIndex_;
    }
    int space() const
    {
        return data_.size() - endIndex_;
    }
    const char* begin() const
    {
        return data_.data() + beginIndex_;
    }
    const char* end() const
    {
        return data_.data() + endIndex_;
    }

    void retrieve(int length)
    {
        if (length <= size())
            beginIndex_ += length;
        else
            endIndex_ = beginIndex_ = pretendedLength;
    }

    int readSocket(const Socket& socket);

  private:
    // 确保有足够的后写空间
    void ableAppend(int length);
    // 确保有足够的前写空间
    void ablePrepend(int length);

    std::vector<char> data_;
    int beginIndex_;
    int endIndex_;
};

#endif // BUFFER_H
