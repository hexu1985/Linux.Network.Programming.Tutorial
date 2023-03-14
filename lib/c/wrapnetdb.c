#include "wrapnetdb.h"
#include <netdb.h>

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

struct hostent *
Gethostbyname(const char *name) {
    struct hostent *hptr;
    if ((hptr = gethostbyname(name)) == NULL) {
        hostent_err_msg(h_errno, "gethostbyname(%s) error", name);
        exit(1);
    }
    return hptr;
}

enum netdb_error_type {
    netdb_hostent_error = 1,
    netdb_addrinfo_error = 2,
};

static void	netdb_err_doit(enum netdb_error_type, int, const char *, va_list);

void
hostent_err_msg(int error_code, const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	netdb_err_doit(netdb_hostent_error, error_code, fmt, ap);
	va_end(ap);
	return;
}

void
addrinfo_err_msg(int error_code, const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	netdb_err_doit(netdb_addrinfo_error, error_code, fmt, ap);
	va_end(ap);
	return;
}

#define MAXLINE 1024

static void
netdb_err_doit(enum netdb_error_type error_type, int error_code, const char *fmt, va_list ap)
{
	int		n;
	char	buf[MAXLINE + 1];

	vsnprintf(buf, MAXLINE, fmt, ap);	/* safe */
	n = strlen(buf);
    switch (error_type) {
    case netdb_hostent_error:
        snprintf(buf + n, MAXLINE - n, ": %s", hstrerror(error_code));
        break;
    case netdb_addrinfo_error:
        snprintf(buf + n, MAXLINE - n, ": %s", gai_strerror(error_code));
        break;
    default:
        break;
    }
	strcat(buf, "\n");

    fflush(stdout);		/* in case stdout and stderr are the same */
    fputs(buf, stderr);
    fflush(stderr);
	return;
}
