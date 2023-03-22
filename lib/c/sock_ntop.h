#pragma once

#include <arpa/inet.h>
#include <sys/un.h>

const char *
Sock_ntop(const struct sockaddr *addr, socklen_t addrlen);

