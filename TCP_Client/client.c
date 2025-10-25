#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFF_SIZE 1024

int main(int argc, char *argv[])
{
    int clientfd;
    char send_buff[BUFF_SIZE];
    char recv_buff[BUFF_SIZE];
    struct sockaddr_in servAddr;

    if (argc != 3)
    {
        printf("Usage: %s <Server_IP> <Server_Port>\n", argv[0]);
        exit(1);
    }

    char *SERVER_IP = argv[1];
    int SERVER_PORT = atoi(argv[2]);

    // Step 1: Construct socket
    clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd < 0)
    {
        perror("socket() error");
        exit(1);
    }

    // Step 2: Connnect
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &servAddr.sin_addr);

    if (connect(clientfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        perror("Error: Cannot connect to server");
        close(clientfd);
        exit(1);
    }

    printf("Connected to server %s:%d\n", SERVER_IP, SERVER_PORT);

    // Step 4: Communicate
    while (1)
    {
        printf("Enter message: ");
        fgets(send_buff, BUFF_SIZE, stdin);

        send_buff[strcspn(send_buff, "\n")] = '\0';

        if (strlen(send_buff) == 0)
            break;

        send(clientfd, send_buff, strlen(send_buff), 0);

        int bytes_received = recv(clientfd, recv_buff, BUFF_SIZE, 0);
        if (bytes_received <= 0)
            break;
        recv_buff[bytes_received] = '\0';

        printf("Reply from server: %s\n", recv_buff);
    }

    close(clientfd);
    printf("Connection closed.\n");
    return 0;
}
