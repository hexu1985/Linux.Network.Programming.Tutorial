#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "error_handling.h"

#define	LINELEN		128

void echo_client(int fd);

int main(int argc, char* argv[])
{
	int                 sockfd;
	struct sockaddr_in  serv_addr;
	
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

    echo_client(sockfd);

	close(sockfd);
	return 0;
}

void echo_client(int fd)
{
	char buf[LINELEN+1];    /* buffer for one line of text	*/
	int	n;			        /* socket descriptor, read count*/
	int	outchars, inchars;	/* characters sent and received	*/

	while (fgets(buf, sizeof(buf), stdin)) {
		buf[LINELEN] = '\0';	/* insure line null-terminated	*/
		outchars = strlen(buf);
		(void) write(fd, buf, outchars);

		/* read it back */
		for (inchars = 0; inchars < outchars; inchars+=n ) {
			n = read(fd, &buf[inchars], outchars - inchars);
			if (n < 0)
				error_handling("read error");
		}
		fputs(buf, stdout);
	}
}
