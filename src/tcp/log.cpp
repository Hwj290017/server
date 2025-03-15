#include "log.h"
#include <ctime>
#include <mutex>
#include <time.h>

Logger::Logger(const char* path, Logger::Target target) : path_(path), target_(target)
{
    outfile_.open(path_);
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
void Logger::flush()
{
    std::lock_guard<std::mutex> lock(mutex_);
    outfile_.flush();
}
Logger Logger::logger(LOGPATH);