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

#define PORT 5500
#define BACKLOG 20
#define BUFF_SIZE 4096

void echo(int sockfd);
void sig_chld(int signo);

int main()
{
    int listen_sock, conn_sock;
    struct sockaddr_in server_addr, client_addr;
    pid_t pid;
    int sin_size;

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

    // Establish a signal handler to catch SIGCHILD
    signal(SIGCHLD, sig_chld);

    printf("Server started at port %d...\n", PORT);

    while (1)
    {
        sin_size = sizeof(struct sockaddr_in);
        if ((conn_sock = accept(listen_sock, (struct sockaddr *)&client_addr, (socklen_t *)&sin_size)) == -1)
        {
            if (errno == EINTR)
                continue;
            perror("accept() error");
            exit(EXIT_FAILURE);
        }

        // Fork a child process to handle new client
        pid = fork();
        if (pid < 0)
        {
            perror("fork() error");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            // Child process
            close(listen_sock);

            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
            int client_port = ntohs(client_addr.sin_port);

            printf("You got a connection from %s:%d\n", client_ip, client_port);

            echo(conn_sock);
            exit(0);
        }

        // Parent closes this socket, child handles it
        close(conn_sock);
    }

    close(listen_sock);
    return 0;
}

// Handle dead child (prevent zombie processes)
void sig_chld(int signo)
{
    pid_t pid;
    int stat;

    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("Child %d terminated\n", pid);
}

// Echo service
void echo(int sockfd)
{
    char buff[BUFF_SIZE];
    int len;

    while (1)
    {
        len = recv(sockfd, buff, BUFF_SIZE, 0);
        if (len < 0)
        {
            perror("recv() error");
            break;
        }
        else if (len == 0)
        {
            printf("Client closed connection.\n");
            break;
        }

        buff[len] = '\0';
        printf("Received: %s\n", buff);

        if (send(sockfd, buff, len, 0) < 0)
        {
            perror("send() error");
            break;
        }
    }

    close(sockfd);
}
