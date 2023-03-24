#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>

void
Gethostname(char *name, size_t len);

int
Fcntl(int fd, int cmd, ... /* arg */ );

int
Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, 
        struct timeval *timeout);
