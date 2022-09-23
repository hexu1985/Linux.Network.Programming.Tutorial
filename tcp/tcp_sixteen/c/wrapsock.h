#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void 
Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void 
Getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

void 
Inet_pton(int family, const char *strptr, void *addrptr);

char *
Sock_ntop(const struct sockaddr *addr, socklen_t addrlen);

int 
Socket(int domain, int type, int protocol);

