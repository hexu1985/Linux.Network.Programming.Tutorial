#pragma once

#include <sys/types.h>
#include <sys/socket.h>

int connect_nonb(int sockfd, const struct sockaddr *addr, socklen_t addrlen, int nsec);

