#include "echo.h"
#include "service.pb.h"

::rpc::OutputEcho echo(::rpc::InputEcho msg)
{
    rpc::OutputEcho res;
    res.set_data(std::move(msg).data());
    return res;
}