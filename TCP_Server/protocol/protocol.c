#include "protocol.h"
#include "../user/user.h"
#include "../auth/auth.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#define MAX_BUFFER 4096

/**
 * @brief Handle protocol communication with a connected client
 * @param sockfd Socket file descriptor for the connected client
 * -description
 * Implements the server-side protocol for handling client commands.
 * Supports USER, POST, and BYE commands with appropriate responses.
 * Manages user login state and processes commands accordingly.
 */

void handle_protocol(int sockfd)
{
    User users[MAX_USERS];
    int user_count = loadAccounts("TCP_Server/account.txt", users, MAX_USERS);

    int logged_in = 0;
    int current_user_index = -1;

    char buffer[MAX_BUFFER] = {0};
    int buffer_len = 0;
    int len;

    // Send welcome message per protocol
    send(sockfd, "100 Welcome to server\n", strlen("100 Welcome to server\n"), 0);

    while ((len = recv(sockfd, buffer + buffer_len, MAX_BUFFER - buffer_len - 1, 0)) > 0)
    {
        buffer_len += len;
        buffer[buffer_len] = '\0';

        char *newline;

        // process full command lines
        while ((newline = strchr(buffer, '\n')) != NULL)
        {
            *newline = '\0'; // terminate one command

            printf("[Client Command] %s\n", buffer);

            char cmd[10] = {0};
            char arg[512] = {0};
            char res[128];

            // Safe parsing
            sscanf(buffer, "%9s %511[^\n]", cmd, arg);

            if (strcmp(cmd, "USER") == 0)
            {
                int code = processUSER(arg, &logged_in, &current_user_index, users, user_count);

                if (code == 110)
                    sprintf(res, "110 - Login successful\n");
                else if (code == 211)
                    sprintf(res, "211 - User blocked\n");
                else if (code == 212)
                    sprintf(res, "212 - User not found\n");
                else if (code == 213)
                    sprintf(res, "213 - Already logged in\n");
                else
                    sprintf(res, "300 - Undefined command\n");

                send(sockfd, res, strlen(res), 0);
            }
            else if (strcmp(cmd, "POST") == 0)
            {
                int code = processPOST(arg, logged_in);

                if (code == 120)
                    sprintf(res, "120 - Post message successful\n");
                else if (code == 221)
                    sprintf(res, "221 - You must login first\n");
                else
                    sprintf(res, "300 - Undefined command\n");

                send(sockfd, res, strlen(res), 0);
            }
            else if (strcmp(cmd, "BYE") == 0)
            {
                int code = processBYE(&logged_in);

                if (code == 130)
                    sprintf(res, "130 - Logout successful\n");
                else if (code == 221)
                    sprintf(res, "221 - You must login first\n");
                else
                    sprintf(res, "300 - Undefined command\n");

                send(sockfd, res, strlen(res), 0);
            }
            else
            {
                sprintf(res, "300 - Undefined command\n");
                send(sockfd, res, strlen(res), 0);
            }

            // shift remaining buffer forward (stream handling)
            int consumed = (newline - buffer) + 1;
            memmove(buffer, buffer + consumed, buffer_len - consumed);
            buffer_len -= consumed;
            buffer[buffer_len] = '\0';
        }
    }

    printf("Client disconnected.\n");
    close(sockfd);
}
