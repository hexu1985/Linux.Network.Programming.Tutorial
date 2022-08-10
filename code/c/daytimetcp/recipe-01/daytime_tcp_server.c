#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "error_handling.h"

#define MAXLINE 128

#define LISTENQ 5

int main(int argc, char *argv[])
{
    int                 listenfd, connfd;
    socklen_t           len;
	struct sockaddr_in  serv_addr, cli_addr;
    char                buff[MAXLINE];
    time_t              ticks;
    int                 n;
	
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd < 0)
		error_handling("socket error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family        = AF_INET;
	serv_addr.sin_addr.s_addr   = htonl(INADDR_ANY);
	serv_addr.sin_port          = htons(atoi(argv[1]));
	
	if(bind(listenfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
		error_handling("bind error"); 
	
	if(listen(listenfd, LISTENQ) < 0)
		error_handling("listen error");
	
    for ( ; ; ) {
        len = sizeof(cli_addr);  
        connfd = accept(listenfd, (struct sockaddr*) &cli_addr, &len);
        if(connfd < 0)
            error_handling("accept error");  

        printf("connection from %s, port %d\n",
               inet_ntop(AF_INET, &cli_addr.sin_addr, buff, sizeof(buff)),
               ntohs(cli_addr.sin_port));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        n = strlen(buff);
        if (write(connfd, buff, n) != n)
            error_handling("write error");

        close(connfd);
    }
	
	close(listenfd);
	return 0;
}

