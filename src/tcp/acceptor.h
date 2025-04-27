#pragma once
#include "inetAddress.h"
#include "ioobject.h"
#include "tcp/acceptorid.h"
#include <cstddef>
#include <memory>
#include <unordered_map>
namespace tcp
{
class Connection;

class Acceptor : public IoObject
{
  public:
    using AfterAcceptTask = std::function<void(Acceptor*, Connection*)>;
    Acceptor(IoContext* context, const InetAddress& addr);
    ~Acceptor();
    void start();
    void afterAccept(AfterAcceptTask task);
    void stop(double delay = 0.0);
    void onRead() override;

  private:
    IoContext* ioContext_;
    InetAddress addr_;
    AfterAcceptTask afterAcceptTask_;
};

} // namespace tcp
