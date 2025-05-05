

#include "iocontextpool.h"
#include "iocontext.h"
#include <functional>
#include <thread>

namespace tcp
{

IoContextPool::IoContextPool(size_t ioContextNum) : ioContextNum_(ioContextNum), ioContexts_(ioContextNum_)
{
}

IoContextPool::~IoContextPool()
{
    for (auto& ioContext : ioContexts_)
    {
        ioContext.first.stop();
        if (ioContext.second.joinable())
        {
            ioContext.second.join();
        }
    }
}
void IoContextPool::start()
{
    for (size_t i = 0; i < ioContextNum_; ++i)
    {
        // 此处需保证ioContext的生命周期
        ioContexts_[i].second = std::thread([ioContext = &ioContexts_[i].first]() { ioContext->start(); });
        indexMap_[std::hash<std::thread::id>()(ioContexts_[i].second.get_id())] = i;
    }
}

// void IoContextPool::stop()
// {
// {
//     for (auto& ioContext : ioContexts_)
//     {
//         ioContext.stop();
//     }
// }

IoContext* IoContextPool::getCurrentIoContext()
{
    // auto index = indexMap_[std::hash<std::thread::id>()(std::this_thread::get_id())];
    // return &ioContexts_[index].first;
    return &ioContexts_[0].first;
}
// void IoContextPool::runTask()
} // namespace tcp