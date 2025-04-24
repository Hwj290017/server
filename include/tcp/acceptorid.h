#pragma once

#include <functional>
#include <memory>
#include <vector>
namespace tcp
{
class ConnectionId;
class IoContextId;
class AcceptorId
{
  public:
    using BeforeAcceptTask = std::function<void(AcceptorId)>;
    using AfterAcceptTask = std::function<void(AcceptorId, ConnectionId)>;
    using AcceptorTask = std::function<void(AcceptorId)>;

    // 在接受连接前执行任务
    void beforeAccept(BeforeAcceptTask task);
    // 在接受连接后执行任务
    void afterAccept(AfterAcceptTask task);
    // 执行任务
    void doTask(AcceptorTask task, double delay = 0.0, double interval = 0.0);
    // 获取事件循环
    void getIoContextId();
    // 设置事件循环
    void setIoContextId(IoContextId id);

  private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace tcp