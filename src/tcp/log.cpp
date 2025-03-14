#include "log.h"
#include <ctime>
#include <time.h>

Logger logger;

Logger::Logger(const char* path) : path_(path)
{
    std::string tmp = ""; // 双引号下的常量不能直接相加，所以用一个std::string类型做转换
    std::string welcome = tmp + "[Welcome] " + " " + time() + "=== Start logging ===\n";
    outfile_.open(path_);
    if (!outfile_.is_open())
        return;
    outfile_ << welcome;
}

Logger::~Logger()
{
}

const char* Logger::time()
{
    time_t second;
    ::time(&second);
    return ctime(&second);
}
