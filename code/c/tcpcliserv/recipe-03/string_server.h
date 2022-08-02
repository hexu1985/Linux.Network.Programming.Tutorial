#pragma once

#include <unistd.h>
#include "write_all.h"
#include "error_handling.h"

#ifndef	MAXLINE
#define	MAXLINE 1024
#endif

static void string_server(int sockfd)
{
	ssize_t		n;
	char		buf[MAXLINE];

again:
	while ( (n = read(sockfd, buf, MAXLINE)) > 0)
		write_all(sockfd, buf, n);

	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
		error_handling("str_echo: read error");
}
