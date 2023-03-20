#pragma once

#include <arpa/inet.h>
#include <unistd.h>
#include <sys/un.h>

void
Gethostname(char *name, size_t len);

void 
Inet_pton(int family, const char *strptr, void *addrptr);

const char *
Inet_ntop(int family, const void *addrptr, char *strptr, socklen_t len);

uint16_t
Sock_port(const struct sockaddr *addr, socklen_t addrlen);

const char *
Sock_ntop(const struct sockaddr *addr, socklen_t addrlen);

