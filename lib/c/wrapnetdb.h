#pragma once

#include <netdb.h>

void    hostent_err_msg(int, const char *, ...);
void    addrinfo_err_msg(int, const char *, ...);

struct hostent *
Gethostbyname(const char *name);

