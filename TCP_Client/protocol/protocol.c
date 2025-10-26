#include "protocol.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#define BUFF_SIZE 1024

void receive_server_message(int sockfd)
{
    char buff[BUFF_SIZE];
    int len = recv(sockfd, buff, BUFF_SIZE - 1, 0);

    if (len > 0)
    {
        buff[len] = '\0';
        printf("Server response: %s\n", buff);
    }
    else if (len == 0)
    {
        printf("Server closed connection.\n");
    }
}

void send_command(int sockfd, const char *cmd)
{
    send(sockfd, cmd, strlen(cmd), 0);
    receive_server_message(sockfd);
}
