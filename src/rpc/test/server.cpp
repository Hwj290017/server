#include "InetAddress.h"
#include "rpcserver.h"
#include "service.h"
#include "services/echo.h"
#include <memory>
int main()
{
    InetAddress serverAddr("127.0.0.1", 8888);
    RpcServer server(serverAddr);
    server.registerService("echo", echo);
    server.start();
}