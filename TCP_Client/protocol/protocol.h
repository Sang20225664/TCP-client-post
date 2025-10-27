#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <sys/socket.h>

#define BUFF_SIZE 1024

// Declare functions used by the client
void receive_server_message(int sockfd);
void send_and_receive(int sockfd, const char *msg);

#endif // PROTOCOL_H
