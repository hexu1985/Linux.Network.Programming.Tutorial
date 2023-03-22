#include "wrapunix.h"
#include "error.h"

#include <stdarg.h>
#include <errno.h>

int 
Fcntl(int fd, int cmd, ...)
{
    va_list ap;
    int iarg;
    void *varg;
    int n;

    va_start(ap, cmd);
    if (cmd == F_SETLKW || cmd == F_SETLK) {
        varg = va_arg(ap, void *);
        n = fcntl(fd, cmd, varg);
    } else {
        iarg = va_arg(ap, int);
        n = fcntl(fd, cmd, iarg);
    }
    va_end(ap);

    if (n == -1) {
		err_sys("fcntl error");
    }
    return(n);
}

int
Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
       struct timeval *timeout)
{
	int	n;

again:
	if ( (n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0) {
		if (errno == EINTR)
			goto again;
		else
			err_sys("select error");
	} else if (n == 0 && timeout == NULL)
		err_quit("select returned 0 with no timeout");
	return(n);		/* can return 0 on timeout */
}
