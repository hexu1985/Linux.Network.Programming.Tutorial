#pragma once

#include <unistd.h>
#include <errno.h>

#include "error_handling.h"

static void write_all(int fd, const void *vptr, size_t n)
{
	size_t		nleft;
	ssize_t		nwritten;
	const char	*ptr;

	ptr = (const char *) vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;		/* and call write() again */
			else
                error_handling("write() error");    /* error */
		}

		nleft -= nwritten;
		ptr   += nwritten;
	}
}
