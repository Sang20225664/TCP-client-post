#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "protocol/protocol.h"

#define BUFF_SIZE 1024

int main(int argc, char *argv[])
{
    int clientfd;
    char command[BUFF_SIZE];
    struct sockaddr_in servAddr;

    if (argc != 3)
    {
        printf("Usage: %s <Server_IP> <Server_Port>\n", argv[0]);
        exit(1);
    }

    char *SERVER_IP = argv[1];
    int SERVER_PORT = atoi(argv[2]);

    clientfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &servAddr.sin_addr);

    connect(clientfd, (struct sockaddr *)&servAddr, sizeof(servAddr));

    printf("Connected to server %s:%d\n", SERVER_IP, SERVER_PORT);

    // Receive welcome message from server (100)
    receive_server_message(clientfd);

    while (1)
    {
        printf("\nEnter command: ");
        fgets(command, BUFF_SIZE, stdin);
        command[strcspn(command, "\n")] = '\0';

        if (strlen(command) == 0)
            break;

        send(clientfd, command, strlen(command), 0);

        // Receive server response
        char response[BUFF_SIZE];
        int len = recv(clientfd, response, BUFF_SIZE - 1, 0);

        if (len <= 0)
        {
            printf("Server closed connection.\n");
            break;
        }

        response[len] = '\0';
        printf("Server response: %s\n", response);
    }

    close(clientfd);
    printf("Connection closed.\n");
    return 0;
}