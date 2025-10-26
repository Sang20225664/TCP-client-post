#include "../user/user.h"
#include <string.h>
#include <stdio.h>

int processUSER(char *username, int *logged_in, int *current_index, User users[], int user_count)
{
    int idx = findUser(username, users, user_count);

    if (*logged_in)
        return 213; // already logged in

    if (idx == -1)
        return 212; // not exist

    if (users[idx].status == 0)
        return 211; // blocked

    *logged_in = 1;
    *current_index = idx;
    return 110; // success
}

int processPOST(char *content, int logged_in)
{
    if (!logged_in)
        return 221; // must login first

    // Write later
    return 120;
}

int processBYE(int *logged_in)
{
    if (!*logged_in)
        return 221;

    *logged_in = 0;
    return 130;
}
