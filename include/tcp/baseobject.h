#pragma once

#include "tcp/tempptr.h"
#include <any>
#include <cstddef>
#include <functional>
#include <memory>
namespace tcp
{
class IoContext;
template <typename T> struct Impl;
template <typename T> class BaseObject
{
  public:
    using Task = std::function<void(TempPtr<T>)>;
    using StartTask = std::function<void(TempPtr<T>)>;
    using StopTask = std::function<void(TempPtr<T>)>;
    using ReleaseTask = std::function<void(std::size_t)>;

    void start(double deley = 0.0);
    void resume(double deley = 0.0);
    void pause(double deley = 0.0);
    void stop(double deley = 0.0);
    void doTask(const Task& task, double deley = 0.0, double interval = 0.0);
    void setContext(std::any context);
    std::any& getContext();
    std::size_t id() const;

  protected:
    std::unique_ptr<Impl<T>> impl_;
};
} // namespace tcp