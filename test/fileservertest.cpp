#include "fileserver.h"
#include "InetAddress.h"
#include <string>

int main()
{
    FileServer server(InetAddress("127.0.0.1", 8888), std::string("fileserver"));
    server.start();
    return 0;
}
