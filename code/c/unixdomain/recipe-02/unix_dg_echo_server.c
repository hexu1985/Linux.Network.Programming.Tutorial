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
    int n, sockfd;
    struct sockaddr_un servaddr, cliaddr;
    char message[BUF_SIZE];
    socklen_t len;

    const char *servpath = UNIXDG_PATH;
    if (argc == 2) {
        servpath = argv[1];
    }

    sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (sockfd < 0)
        error_handling("socket() error");

    unlink(servpath);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, servpath);

    if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        error_handling("bind() error");

    while(1)
    {
        len = sizeof(cliaddr);
        n = recvfrom(sockfd, message, BUF_SIZE, 0,
                (struct sockaddr*) &cliaddr, &len);
        sendto(sockfd, message, n, 0,
                (struct sockaddr*) &cliaddr, len);
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
