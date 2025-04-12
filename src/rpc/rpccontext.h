#pragma once
#include "buffer.h"
class Buffer;
class Service;
class RpcContext
{
  public:
    void parseService(const Buffer& buffer);
    void isRight();

  private:
    bool isRight_;
    Service service_;
};
