#pragma once

#include <stdint.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <linux/netlink.h>

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

const char *
Sock_ntop(const struct sockaddr *addr, socklen_t addrlen);

int 
Socket(int family, int type, int protocol);

void
Send(int sockfd, const void *buf, size_t len, int flags);

void
SendAll(int sockfd, const void *ptr, size_t nbytes);

void
SendMsg(int sockfd, const char *msg);

void
RecvAll(int sockfd, void *ptr, size_t nbytes);

void
Shutdown(int sockfd, int how);

void
Sendto(int sockfd, const void *buf, size_t len, int flags,
        const struct sockaddr *dest_addr, socklen_t addrlen);

ssize_t 
Recvfrom(int sockfd, void *buf, size_t len, int flags,
        struct sockaddr *src_addr, socklen_t *addrlen);

void
Gethostname(char *name, size_t len);

const char *
Inet_ntop(int family, const void *addrptr, char *strptr, socklen_t len);

uint16_t
Sock_port(const struct sockaddr *addr, socklen_t addrlen);
