#pragma once

#include <any>
#include <functional>
#include <memory>
namespace tcp
{

class Object
{
  public:
    using Task = std::function<void()>;
    using StartTask = std::function<void(Object*)>;
    using StopTask = std::function<void(Object*)>;
    struct Tasks
    {
        StartTask startTask;
        StopTask stopTask;
    };
    // 没有实现start，创建即开始
    void resume(double deley = 0.0);
    void pause(double deley = 0.0);
    void stop(double deley = 0.0);
    void doTask(Task task, double deley = 0.0, double interval = 0.0);
    void setContext(std::any context);
    std::any& getContext();

  protected:
    Object(const Tasks& tasks);
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace tcp