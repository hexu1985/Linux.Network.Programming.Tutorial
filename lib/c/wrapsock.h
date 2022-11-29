#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "error.h"

int 
Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

void 
Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void
Close(int fd);

void 
Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void 
Getpeername(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

void 
Getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

void 
Inet_pton(int family, const char *strptr, void *addrptr);

void 
Listen(int sockfd, int backlog);

ssize_t 
Recv(int sockfd, void *buf, size_t len, int flags);

void
Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

char *
Sock_ntop(const struct sockaddr *addr, socklen_t addrlen);

int 
Socket(int family, int type, int protocol);

void
SendAll(int fd, const void *ptr, size_t nbytes);

void
RecvAll(int fd, void *ptr, size_t nbytes);


