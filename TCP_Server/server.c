#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <errno.h>

#include "protocol/protocol.h"

#define BACKLOG 20
#define BUFF_SIZE 4096

void sig_chld(int signo);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <Server_Port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int PORT = atoi(argv[1]);
    if (PORT <= 0)
    {
        printf("Invalid port number.\n");
        exit(EXIT_FAILURE);
    }

    int listen_sock, conn_sock;
    struct sockaddr_in server_addr, client_addr;
    pid_t pid;
    socklen_t sin_size;

    // Create socket
    if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket() error");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listen_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind() error");
        exit(EXIT_FAILURE);
    }

    if (listen(listen_sock, BACKLOG) == -1)
    {
        perror("listen() error");
        exit(EXIT_FAILURE);
    }

    signal(SIGCHLD, sig_chld);

    printf("Server started at port %d...\n", PORT);

    while (1)
    {
        sin_size = sizeof(client_addr);
        conn_sock = accept(listen_sock, (struct sockaddr *)&client_addr, &sin_size);
        if (conn_sock == -1)
        {
            if (errno == EINTR)
                continue;
            perror("accept() error");
            continue;
        }

        pid = fork();
        if (pid < 0)
        {
            perror("fork() error");
            continue;
        }

        if (pid == 0)
        {
            // Child process
            close(listen_sock);

            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
            int client_port = ntohs(client_addr.sin_port);

            printf("You got a connection from %s:%d\n", client_ip, client_port);

            handle_protocol(conn_sock);
            close(conn_sock);
            exit(0);
        }

        close(conn_sock);
    }

    close(listen_sock);
    return 0;
}

void sig_chld(int signo)
{
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
}
