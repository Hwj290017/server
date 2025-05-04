#pragma once

#include "iocontext.h"
#include <cstddef>
#include <functional>
#include <vector>
namespace tcp
{

class IoContextPool
{
  public:
    IoContextPool(std::size_t ioContextNum);
    ~IoContextPool();
    void start();
    void setIoContextNum(std::size_t ioContextNum);
    std::size_t getIoContextNum() const;
    IoContext* getCurrentIoContext();

    IoContext* getIoContext(std::size_t ioContextId);
    IoContext* getAttachedIoContext(std::size_t baseObjectId);
    // void runTask(std::)

  private:
    std::vector<IoContext> ioContexts_;
    std::size_t ioContextNum_;
    // static size_t ioContextId_;
};
} // namespace tcp