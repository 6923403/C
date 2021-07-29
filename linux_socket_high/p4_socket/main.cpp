/*
 * https://blog.csdn.net/jasonliuvip/category_2153209.html
 */
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <cassert>
#include <netinet/in.h>

const int BUFFER_SIZE = 1024;

int main(int argc, char **argv)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_addr.sin_port = htons(20999);

    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    int recvbuf = 3000;
    int len = sizeof(recvbuf);
    //设置接受缓冲区大小
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &recvbuf, sizeof(recvbuf));
    //获取系统修改后的大小
    getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &recvbuf, (socklen_t*)&len);
    printf("the receive buffer size after setting is %d\n", recvbuf);

    int ret = bind(sockfd, (struct sockaddr*)&s_addr, sizeof(s_addr));
    assert(ret != -1);

    ret = listen(sockfd, 5);
    assert(ret != -1);

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    while(true) {
        int connfd = accept(sockfd, (struct sockaddr *) &client_addr, &client_len);
        if (connfd < 0) {
            printf("errno is: %d\n", errno);
        } else {
            char buffer[BUFFER_SIZE];
            memset(buffer, '\0', BUFFER_SIZE);
            strcpy(buffer, "");
            //recv(connfd, buffer, BUFFER_SIZE - 1, 0);
            //printf("recv: %s\n", buffer);
            send(connfd, buffer, BUFFER_SIZE -1, 0);
            close(connfd);
        }
    }

    close(sockfd);

    return 0;
}