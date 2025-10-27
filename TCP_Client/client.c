#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "protocol/protocol.h"

#define BUFF_SIZE 1024

/**
 * @brief TCP Client Application
 * Connects to a TCP server, allows user to login, post messages, and logout.
 *
 */

int main(int argc, char *argv[])
{
    int clientfd;
    struct sockaddr_in servAddr;

    if (argc != 3)
    {
        printf("Usage: %s <Server_IP> <Server_Port>\n", argv[0]);
        exit(1);
    }

    char *SERVER_IP = argv[1];
    int SERVER_PORT = atoi(argv[2]);

    clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &servAddr.sin_addr);

    if (connect(clientfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        perror("Connection to server failed");
        exit(1);
    }

    printf("Connected to server %s:%d\n", SERVER_IP, SERVER_PORT);

    // Receive welcome message from server (100)
    send_and_receive(clientfd, "");

    while (1)
    {
        printf("====== MENU ======\n");
        printf("1. Login\n");
        printf("2. Post Message\n");
        printf("3. Logout\n");
        printf("4. Exit\n");
        printf("Enter choice: ");

        int choice;
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input.\n\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        while (getchar() != '\n')
            ; // drop newline

        if (choice == 1)
        {
            char username[100];
            printf("Enter username: ");
            fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = '\0';

            char msg[128];
            snprintf(msg, sizeof(msg), "USER %s", username);
            send_and_receive(clientfd, msg);
        }
        else if (choice == 2)
        {
            char content[512];
            printf("Enter post message: ");
            fgets(content, sizeof(content), stdin);
            content[strcspn(content, "\n")] = '\0';

            char msg[600];
            snprintf(msg, sizeof(msg), "POST %s", content);
            send_and_receive(clientfd, msg);
        }
        else if (choice == 3)
        {
            send_and_receive(clientfd, "BYE");
        }
        else if (choice == 4)
        {
            printf("Bye!\n");
            break;
        }
        else
        {
            printf("Invalid choice.\n\n");
        }
    }

    close(clientfd);
    printf("Connection closed.\n");
    return 0;
}