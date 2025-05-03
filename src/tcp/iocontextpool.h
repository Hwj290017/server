#pragma once

#include "iocontext.h"
#include <cstddef>
#include <memory>
#include <mutex>
#include <vector>
namespace tcp
{
class IoContextPool
{
  public:
    using IoContextPtr = std::unique_ptr<IoContext>;
    static constexpr std::size_t kInitialIoContextNum = 8;

    void start();
    static IoContextPool& instance();

  private:
    IoContextPool(std::size_t ioContextNum = kInitialIoContextNum);
    ~IoContextPool();
    std::size_t ioContextNum_;
    std::vector<IoContextPtr> ioContexts_;
    std::mutex mutex_;
    std::size_t currentIndex_;
};
} // namespace tcp
