#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <errno.h>
#include <signal.h>

#include "error_handling.h"
#include "signal_handling.h"
#include "string_server.h"

#define QLEN          32    /* maximum connection queue length  */

int main(int argc, char **argv)
{
	int					listenfd, connfd;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;
	void				sig_chld(int);
    short               serv_port;

	if (argc != 2)
        error_handling("usage: %s <Port>", argv[0]);

    serv_port = atoi(argv[1]);

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
        error_handling("socket() error");

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(serv_port);

	if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        error_handling("bind() error");

	if (listen(listenfd, QLEN) < 0)
        error_handling("listen() error");

	signal(SIGCHLD, signal_handling);

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		if ( (connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0) {
			if (errno == EINTR)
				continue;		/* back to for() */
			else
				error_handling("accept() error");
		}

		switch (fork()) {
		case 0:		/* child process */
			close(listenfd);	    /* close listening socket */
			string_server(connfd);	/* process the request */
			exit(0);
		default:	/* parent */
            close(connfd);			/* parent closes connected socket */
			break;
		case -1:
			error_handling("fork() error");
		}
	}
}
