#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "error_handling.h"

#define MAXLINE 128

int main(int argc, char* argv[])
{
	int                 sockfd;
	struct sockaddr_in  serv_addr;
	char                message[MAXLINE];
	int                 n;
	
	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		error_handling("socket error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family        = AF_INET;
	serv_addr.sin_addr.s_addr   = inet_addr(argv[1]);
	serv_addr.sin_port          = htons(atoi(argv[2]));
		
	if(connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) 
		error_handling("connect error!");

    while ( (n = read(sockfd, message, MAXLINE)) > 0) {
        message[n] = 0;    /* null terminate */
        if (fputs(message, stdout) == EOF)
            error_handling("fputs error");
    }
    if (n < 0)
        error_handling("read error!");

	close(sockfd);
	return 0;
}

