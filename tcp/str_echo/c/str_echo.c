#include "str_echo.h"
#include "unp.h"

#include <errno.h>

void
str_echo(int sockfd)
{
	ssize_t		n;
	char		buf[1024];

again:
	while ( (n = read(sockfd, buf, sizeof(buf))) > 0)
		SendAll(sockfd, buf, n);

	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
		err_sys("str_echo: read error");
}
