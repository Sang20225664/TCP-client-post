#ifndef PROTOCOL_H
#define PROTOCOL_H

#define CODE_WELCOME "100"
#define CODE_LOGIN_OK "110"
#define CODE_USER_NOT_FOUND "212"
#define CODE_USER_BLOCKED "211"
#define CODE_ALREADY_LOGIN "213"
#define CODE_NEED_LOGIN "221"
#define CODE_POST_OK "120"
#define CODE_LOGOUT_OK "130"
#define CODE_UNKNOWN_CMD "300"

#include "../user/user.h"
#include "../auth/auth.h"

void handle_protocol(int sockfd);

#endif
