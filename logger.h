#ifndef LOGGER_H
#define LOGGER_H
#include <arpa/inet.h>

void write_access_log(struct sockaddr_in *client_addr, const char *request_line);

#endif
