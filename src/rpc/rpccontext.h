#pragma once

#include "service.pb.h"
class Buffer;
class RpcContext
{
  public:
    RpcContext() : isRight_(false)
    {
    }

    ::rpc::Request parseRequset(const Buffer& buffer);
    bool isRight()
    {
        return isRight_;
    }

  private:
    bool isRight_;
};
