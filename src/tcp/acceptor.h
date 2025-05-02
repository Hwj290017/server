#pragma once
#include "sharedobject.h"
#include "tcp/inetAddress.h"
#include <cstddef>
namespace tcp
{
class IoContext;

class Acceptor : public SharedObject
{
  public:
    Acceptor(std::size_t id, IoContext* context, std::size_t parent, const InetAddress& addr);
    ~Acceptor();
    // 由pool在自己线程执行，以下线程不安全
    void start();
    void onRead() override;
    void stop();

  private:
    InetAddress addr_;
};

} // namespace tcp
