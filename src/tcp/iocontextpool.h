#pragma once

#include "iocontext.h"
#include <condition_variable>
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
    IoContextPool(std::size_t ioContextNum = kInitialIoContextNum);
    ~IoContextPool();
    // 获取一个与当前线程不同的IoContext
    IoContext* getIoContext();
    // void set(IoContext* ioContext);
    void start();
    static IoContextPool* instance();
    void addConnection(int clientSocket, InetAddress clientAddr, Acceptor* acceptor);
    void addAcceptor(InetAddress listenAddr);

  private:
    std::size_t ioContextNum_;
    std::vector<IoContextPtr> ioContexts_;
    std::mutex mutex_;
    std::size_t currentIndex_;
};
} // namespace tcp
