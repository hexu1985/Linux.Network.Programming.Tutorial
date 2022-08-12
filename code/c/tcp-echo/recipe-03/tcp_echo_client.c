#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "error_handling.h"
#include "echo_client.h"

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;
    const char         *serv_ip;
    short               serv_port;

	if (argc != 3)
        error_handling("usage: %s <IPaddress> <Port>", argv[0]);

    serv_ip = argv[1];
    serv_port = atoi(argv[2]);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		error_handling("socket() error");

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(serv_port);
	if (inet_pton(AF_INET, serv_ip, &servaddr.sin_addr) < 0)
        error_handling("inet_pton() error for %s", serv_ip);

	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr)) < 0)
        error_handling("connect() error");

	echo_client(stdin, sockfd);		/* do it all */

    return 0;
}
