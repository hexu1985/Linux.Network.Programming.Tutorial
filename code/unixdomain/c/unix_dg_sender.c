#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>

#define UNIXDG_PATH "/tmp/unix.dg"
#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_un cliaddr, servaddr;
    char message[BUF_SIZE];

    const char *servpath = UNIXDG_PATH;
    if (argc == 2) {
        servpath = argv[1];
    }

    sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (sockfd < 0)
        error_handling("socket() error");

    memset(&cliaddr, 0, sizeof(cliaddr));        /* bind an address for us */
    cliaddr.sun_family = AF_LOCAL;
    strcpy(cliaddr.sun_path, tmpnam(NULL));

    if (bind(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0)
        error_handling("bind() error");

    memset(&servaddr, 0, sizeof(servaddr));    /* fill in server's address */
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, servpath);

    while(1)
    {
        fputs("Insert message(q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);
        if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
            break;

        sendto(sockfd, message, strlen(message), 0, 
                    (struct sockaddr*)&servaddr, sizeof(servaddr));
    }
    close(sockfd);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
