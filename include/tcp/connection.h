#pragma once
#include "tcp/object.h"
#include "tcp/tempptr.h"
#include <any>
#include <cstddef>
#include <functional>
#include <memory>
namespace tcp
{
class Connection : public Object
{
  public:
    using MessageTask = std::function<void(TempPtr<Connection>, const void*, std::size_t)>;
    using StartTask = std::function<void(TempPtr<Connection>)>;
    using StopTask = std::function<void(TempPtr<Connection>)>;
    struct Tasks
    {
        StartTask startTask;
        MessageTask messageTask;
        StopTask stopTask;
    };

    ~Connection();
    void send(const void* data, std::size_t size);
    void setContext(std::any context);
    std::any& getContext() const;

  private:
    explicit Connection(int clientfd, const Tasks& tasks);
    struct Impl;
    std::unique_ptr<Impl> impl_;
    friend class Manager;
};

} // namespace tcp