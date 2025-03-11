#include "src/util.h"
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 32

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create error");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    errif(connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket connect error");

    while (true)
    {
        char buf[BUFFER_SIZE]; // 在这个版本，buf大小必须大于或等于服务器端buf大小，不然会出错，想想为什么？
        bzero(&buf, sizeof(buf));
        int num = scanf("%s", buf);
        ssize_t write_bytes = write(sockfd, buf, BUFFER_SIZE);
        if (write_bytes == -1)
        {
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        bzero(&buf, sizeof(buf));
    }
    close(sockfd);
    return 0;
}
