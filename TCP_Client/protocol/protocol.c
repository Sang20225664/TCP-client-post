#include "protocol.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>

/**
 * @brief Send a message to the server and receive the response
 * @param sockfd The socket file descriptor
 * @param msg The message to send
 * @return void
 * - Description
 * Sends a message to the server using the provided socket file descriptor.
 * Waits to receive a response from the server and prints it to the console.
 */
void send_and_receive(int sockfd, const char *msg)
{
    char recv_buff[BUFF_SIZE];
    send(sockfd, msg, strlen(msg), 0);

    int bytes = recv(sockfd, recv_buff, BUFF_SIZE - 1, 0);
    if (bytes > 0)
    {
        recv_buff[bytes] = '\0';
        printf("Server response: %s\n\n", recv_buff);
    }
}