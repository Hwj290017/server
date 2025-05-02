#pragma once

#include <functional>
namespace tcp
{

class IoContextId
{
  public:
    using Task = std::function<void(const IoContextId&)>;
    void doTask(Task task, double delay, double interval);
};

} // namespace tcp