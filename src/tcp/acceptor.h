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
    Acceptor(IoContext* context, std::size_t id, const InetAddress& addr);
    ~Acceptor();
    // 调用方保证线程安全
    void onRead() override;
    void setNewConnectionCb(const NewConnectionCallback& cb);

  private:
    InetAddress addr_;
    NewConnectionCallback newConnectionCallback_;
};

} // namespace tcp
