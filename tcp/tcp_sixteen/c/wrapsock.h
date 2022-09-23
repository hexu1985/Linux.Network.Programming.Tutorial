#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void
Close(int fd);

void 
Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void 
Getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

void 
Inet_pton(int family, const char *strptr, void *addrptr);

void
Readn(int fd, void *ptr, size_t nbytes);

char *
Sock_ntop(const struct sockaddr *addr, socklen_t addrlen);

int 
Socket(int domain, int type, int protocol);

void
Writen(int fd, const void *ptr, size_t nbytes);

