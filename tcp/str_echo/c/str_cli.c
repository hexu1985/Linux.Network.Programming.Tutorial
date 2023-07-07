#include "str_cli.h"
#include "unp.h"

void
str_cli(FILE *fp, int sockfd)
{
	char	sendline[1024], recvline[1024];

	while (Fgets(sendline, sizeof(sendline), fp) != NULL) {

		SendAll(sockfd, sendline, strlen(sendline));

		if (Readline(sockfd, recvline, sizeof(recvline)) == 0)
			err_quit("str_cli: server terminated prematurely");

		Fputs(recvline, stdout);
	}
}
