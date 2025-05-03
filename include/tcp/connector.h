#pragma once
#include "tcp/inetaddress.h"
#include "tcp/object.h"
#include "tcp/tempptr.h"
#include <any>
#include <cstddef>
#include <functional>
#include <memory>

namespace tcp
{
class Connector : public Object
{
  public:
    using StartTask = std::function<void(TempPtr<Connector>)>;
    using MessageTask = std::function<void(TempPtr<Connector>, const void*, std::size_t)>;
    using StopTask = std::function<void(TempPtr<Connector>)>;

    struct Tasks
    {
        StartTask startTask;
        MessageTask magssageTask;
        StopTask stopTask;
    };

    ~Connector();
    void send(const void* data, size_t len);
    void setContext(std::any context);
    std::any& getContext();

  private:
    explicit Connector(const InetAddress& serverAddr, const Tasks& tasks);
    struct Impl;
    std::unique_ptr<Impl> impl_;
    friend class Manager;
};

} // namespace tcp