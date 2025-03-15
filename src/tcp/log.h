#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <mutex>
#include <string>

#define LOGPATH "../log/1.log"
class Logger
{
  public:
    enum Target
    {
        Terminal,
        File,
        Both
    };
    Logger(const char* path, Target target = Both);
    ~Logger();
    template <typename T> Logger& operator<<(const T&);
    void flush();
    static Logger logger;

  private:
    // 将日志输出到文件的流对象
    std::ofstream outfile_;
    // 日志文件路径
    const std::string path_;
    Target target_;
    std::mutex mutex_;
    const char* time();
};

template <typename T> Logger& Logger::operator<<(const T& text)
{
    std::lock_guard<std::mutex> lock(mutex_);
    outfile_ << text;
    return *this;
}
#endif
