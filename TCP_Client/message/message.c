#include "message.h"
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>

int send_message(int sockfd, const char *msg)
{
    int len = strlen(msg);
    int ret = send(sockfd, msg, len, 0);
    if (ret <= 0)
    {
        perror("send() error");
        return -1;
    }
    return ret;
}

int recv_message(int sockfd, char *buff, int buff_size)
{
    int ret = recv(sockfd, buff, buff_size - 1, 0);
    if (ret <= 0)
    {
        return ret;
    }
    buff[ret] = '\0';
    return ret;
}
