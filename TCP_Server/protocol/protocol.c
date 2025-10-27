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

    send(sockfd, "100 Welcome to server", 21, 0);

    while ((len = recv(sockfd, buff, sizeof(buff) - 1, 0)) > 0)
    {
        buff[len] = '\0';

        char cmd[10] = {0};
        char arg[512] = {0};
        char res[128];

        sscanf(buff, "%9s %511[^\n]", cmd, arg);

        printf("[Client] %s\n", buff);

        if (strcmp(cmd, "USER") == 0)
        {
            int code = processUSER(arg, &logged_in, &current_user_index, users, user_count);

            if (code == 110)
                sprintf(res, "110 - Login successful");
            else if (code == 211)
                sprintf(res, "211 - User blocked");
            else if (code == 212)
                sprintf(res, "212 - User not found");
            else if (code == 213)
                sprintf(res, "213 - Login failed! Already logged in");
            else
                sprintf(res, "300 - Undefined error");

            send(sockfd, res, strlen(res), 0);
        }
        else if (strcmp(cmd, "POST") == 0)
        {
            int code = processPOST(arg, logged_in);

            if (code == 120)
                sprintf(res, "120 - Post message successful");
            else if (code == 221)
                sprintf(res, "221 - You must login first");
            else
                sprintf(res, "300 - Undefined command");

            send(sockfd, res, strlen(res), 0);
        }
        else if (strcmp(cmd, "BYE") == 0)
        {
            int code = processBYE(&logged_in);

            if (code == 130)
                sprintf(res, "130 - Logout successful");
            else if (code == 221)
                sprintf(res, "221 - You must login first");
            else
                sprintf(res, "300 - Undefined command");

            send(sockfd, res, strlen(res), 0);
        }
        else
        {
            sprintf(res, "300 - Undefined command");
            send(sockfd, res, strlen(res), 0);
        }
    }

    printf("Client disconnected.\n");
    close(sockfd);
}
