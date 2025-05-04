#pragma once

#include "iocontext.h"
#include <cstddef>
#include <unordered_map>
#include <vector>
namespace tcp
{
using size_t = std::size_t;
class IoContextPool
{
  public:
    constexpr static size_t kDefaultIoContextNum = 4;
    IoContextPool(std::size_t ioContextNum = kDefaultIoContextNum);
    ~IoContextPool();
    void start();
    void stop();
    void setIoContextNum(std::size_t ioContextNum);
    std::size_t getIoContextNum() const;
    IoContext* getCurrentIoContext();

    IoContext* getIoContext(std::size_t ioContextId);
    IoContext* getAttachedIoContext(std::size_t baseObjectId);
    // void runTask(std::)

  private:
    std::size_t ioContextNum_;
    std::unordered_map<size_t, size_t> indexMap_;
    std::vector<std::pair<IoContext, std::thread>> ioContexts_;
    // static size_t ioContextId_;
};
} // namespace tcp