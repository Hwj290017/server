#pragma once

#include <cstddef>
#include <functional>
#include <memory>
namespace tcp
{

class IoContextId
{
  public:
    using Task = std::function<void()>;
    IoContextId();
    void runTask(const Task& task, double delay = 0.0, double interval = 0.0);
    std::size_t id();

  private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace tcp