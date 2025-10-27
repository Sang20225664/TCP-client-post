#include "../user/user.h"
#include <string.h>
#include <stdio.h>

/**
 * @brief Process USER command
 * @param username The username provided by the client
 * @param logged_in Pointer to logged_in status
 * @param current_index Pointer to current user index
 * @param users Array of User structs
 * @param user_count Number of users in the array
 * @return int Status code
 */

int processUSER(char *username, int *logged_in, int *current_index, User users[], int user_count)
{
    if (username == NULL || strlen(username) == 0)
        return 300; // wrong format

    int idx = findUser(username, users, user_count);

    if (*logged_in)
    {
        return 213; // already logged in
    }

    if (idx == -1)
        return 212; // not exist

    if (users[idx].status == 0)
        return 211; // blocked

    *logged_in = 1;
    *current_index = idx;
    return 110; // success
}

/**
 * @brief Process POST command
 * @param content The content to post
 * @param logged_in Logged in status
 * @return int Status code
 */

int processPOST(char *content, int logged_in)
{
    if (!logged_in)
        return 221; // must login first

    // Write later
    return 120;
}

/**
 * @brief Process BYE command
 * @param logged_in Pointer to logged_in status
 * @return int Status code
 */

int processBYE(int *logged_in)
{
    if (!*logged_in)
        return 221;

    *logged_in = 0;
    return 130;
}
