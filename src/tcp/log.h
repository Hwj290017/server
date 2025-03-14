#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <string>

#define LOGPATH "../log/1.log"
class Logger
{
  private:
    // 将日志输出到文件的流对象
    std::ofstream outfile_;
    // 日志文件路径
    const std::string path_;
    const char* time();

  public:
    Logger(const char* path = LOGPATH);
    ~Logger();
    template <typename T> Logger& operator<<(T);
};

template <typename T> Logger& Logger::operator<<(T text)
{
    outfile_ << text;
    return *this;
}

// 全局Logger变量
extern Logger logger;
#endif
