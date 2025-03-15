#include "util.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#define BUFFER_SIZE 32

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create error");

    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    errif(connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket connect error");

    while (true)
    {
        std::string input;
        std::getline(std::cin, input);
        ssize_t write_bytes = write(sockfd, input.c_str(), input.size());
        if (write_bytes == -1)
        {
            std::cout << "write error" << std::endl;
        }
        // char buf[BUFFER_SIZE];
        // ssize_t read_bytes = read(sockfd, buf, BUFFER_SIZE);
        // if (read_bytes == 0)
        // {
        //     std::cout << "read error" << std::endl;
        // }
        // std::cout << std::string(buf, read_bytes) << std::endl;
    }
    close(sockfd);
    return 0;
}
