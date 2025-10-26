#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <unistd.h>

void receive_server_message(int sockfd);
void send_command(int sockfd, const char *cmd);

#endif
