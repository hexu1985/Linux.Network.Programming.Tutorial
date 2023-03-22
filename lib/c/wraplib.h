#pragma once

#include <arpa/inet.h>

void 
Inet_pton(int family, const char *strptr, void *addrptr);

const char *
Inet_ntop(int family, const void *addrptr, char *strptr, socklen_t len);

