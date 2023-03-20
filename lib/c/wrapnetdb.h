#pragma once

#include <netdb.h>

void    hostent_err_msg(int, const char *, ...);
void    addrinfo_err_msg(int, const char *, ...);

struct hostent *
Gethostbyname(const char *name);

void 
Getaddrinfo(const char *node, const char *service,
        const struct addrinfo *hints, struct addrinfo **res);

void 
Freeaddrinfo(struct addrinfo *res);
