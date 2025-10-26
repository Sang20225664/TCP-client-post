#include "protocol.h"
#include "../user/user.h"
#include "../auth/auth.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

void handle_protocol(int sockfd)
{
    User users[MAX_USERS];
    int user_count = loadAccounts("TCP_Server/account.txt", users, MAX_USERS);

    int logged_in = 0;
    int current_user_index = -1;

    char buff[4096];
    int len;

    // Send welcome code to client
    send(sockfd, CODE_WELCOME, strlen(CODE_WELCOME), 0);

    while ((len = recv(sockfd, buff, sizeof(buff) - 1, 0)) > 0)
    {
        buff[len] = '\0';

        char *newline = strchr(buff, '\r');
        if (newline)
            *newline = '\0';
        newline = strchr(buff, '\n');
        if (newline)
            *newline = '\0';

        printf("[Client] %s\n", buff);

        char cmd[10] = {0};
        char arg[512] = {0};

        if (sscanf(buff, "%9s %511[^\n]", cmd, arg) < 1)
        {
            send(sockfd, CODE_UNKNOWN_CMD, strlen(CODE_UNKNOWN_CMD), 0);
            continue;
        }

        if (strcmp(cmd, "USER") == 0)
        {
            int code = processUSER(arg, &logged_in, &current_user_index, users, user_count);
            char res[8];
            sprintf(res, "%d", code);
            send(sockfd, res, strlen(res), 0);
        }
        else if (strcmp(cmd, "POST") == 0)
        {
            int code = processPOST(arg, logged_in);
            char res[8];
            sprintf(res, "%d", code);
            send(sockfd, res, strlen(res), 0);
        }
        else if (strcmp(cmd, "BYE") == 0)
        {
            int code = processBYE(&logged_in);
            char res[8];
            sprintf(res, "%d", code);
            send(sockfd, res, strlen(res), 0);
        }
        else
        {
            send(sockfd, CODE_UNKNOWN_CMD, strlen(CODE_UNKNOWN_CMD), 0);
        }
    }

    printf("Client disconnected.\n");
    close(sockfd);
}
