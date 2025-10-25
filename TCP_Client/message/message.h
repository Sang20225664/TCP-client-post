#ifndef MESSAGE_H
#define MESSAGE_H

int send_message(int sockfd, const char *msg);
int recv_message(int sockfd, char *buff, int buff_size);

#endif
