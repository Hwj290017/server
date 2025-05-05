
#include <arpa/inet.h>
#include <assert.h>
#include <cstdio>
#include <iostream>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
int main()
{
    // 创建套接子
    // AF_INFT表示ipv4  SOCKET_STREAM表示传输层使用tcp协议
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd != -1);
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(8888);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int res = connect(sockfd, (struct sockaddr*)&saddr, sizeof(saddr));
    assert(res != -1);
    while (true)
    {
        std::string str;
        char buf[128];
        std::cin >> str;
        int readNum = send(sockfd, str.data(), str.size(), 0);
        if (readNum == -1)
        {
            break;
        }
        memset(buf, 0, 128);
        int writeNum = recv(sockfd, buf, 127, 0);
        if (writeNum == -1)
        {
            break;
        }
        buf[writeNum] = '\0';
        printf("%s\n", buf);
    }
    close(sockfd);

    exit(0);
}