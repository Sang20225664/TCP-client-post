#ifndef AUTH_H
#define AUTH_H

int processUSER(char *username, int *logged_in, int *current_index, User users[], int user_count);
int processPOST(char *content, int logged_in);
int processBYE(int *logged_in);

#endif
