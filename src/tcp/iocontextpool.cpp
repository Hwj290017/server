#include "iocontextpool.h"
#include "iocontext.h"
#include <cstddef>
#include <memory>

namespace tcp
{
IoContextPool::IoContextPool(std::size_t ioContextNum) : ioContextNum_(ioContextNum)
{
}

// 析构函数，用于销毁IoContext对象
IoContextPool::~IoContextPool()
{
}

void IoContextPool::start()
{
    for (auto i = 0; i < ioContextNum_; ++i)
    {
        ioContexts_.emplace_back(std::make_unique<IoContext>());
        ioContexts_.back()->start();
    }
}

IoContext* IoContextPool::getIoContext()
{
    currentIndex_ = (currentIndex_ + 1) % ioContextNum_;
    return ioContexts_[currentIndex_].get();
}

IoContextPool* IoContextPool::instance()
{
    static IoContextPool pool;
    return &pool;
}
} // namespace tcp
