#pragma once
#include "sharedobject.h"
#include "tcp/inetaddress.h"
#include <cstddef>
#include <functional>

namespace tcp
{
class IoContext;

class Acceptor : public SharedObject
{
  public:
    using NewConnectionCallback = std::function<int(int, const InetAddress&)>;
    Acceptor(IoContext* context, std::size_t id, std::size_t parent, const InetAddress& addr);
    ~Acceptor();
    // 由pool在自己线程执行，以下线程不安全
    void start();
    void onRead() override;
    void setNewConnectionCb(const NewConnectionCallback& cb);
    void stop();

  private:
    InetAddress addr_;
    NewConnectionCallback newConnectionCallback_;
};

} // namespace tcp
