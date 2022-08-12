#pragma once

#include <stdio.h>
#include "error_handling.h"
#include "write_all.h"
#include "read_line.h"

#ifndef	MAXLINE
#define	MAXLINE 1024
#endif

static void echo_client(FILE *fp, int sockfd)
{
	char sendline[MAXLINE], recvline[MAXLINE];

	while (fgets(sendline, MAXLINE, fp) != NULL) {

		write_all(sockfd, sendline, strlen(sendline));

		if (read_line(sockfd, recvline, MAXLINE) == 0)
			error_handling("str_cli: server terminated prematurely");

		fputs(recvline, stdout);
	}
}
