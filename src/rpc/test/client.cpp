#include "InetAddress.h"
#include "rpcclient.h"
#include "service.pb.h"
#include <iostream>
int main()
{
    InetAddress serverAddr("127.0.0.1", 8888);
    RpcClient client;
    client.connect(serverAddr);
    rpc::InputEcho input;
    input.set_data("hello\n");
    std::cout << client.call<rpc::OutputEcho>("echo", input).SerializeAsString();
}