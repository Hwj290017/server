#include "rpccontext.h"
#include "buffer.h"
#include "service.pb.h"
::rpc::Request RpcContext::parseRequset(const Buffer& buffer)
{
    auto request = ::rpc::Request();
    request.ParseFromString(buffer.toString());
    isRight_ = true;
    return request;
}