#ifndef HANDLER_H
#define HANDLER_H
#include <netinet/in.h>

void handle_client(int client_fd, struct sockaddr_in *addr);

#endif
