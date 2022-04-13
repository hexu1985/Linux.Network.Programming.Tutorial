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
    struct sockaddr_un servaddr;
    char message[BUF_SIZE];

    sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
    if (sockfd < 0)
        error_handling("socket() error");

    unlink(UNIXDG_PATH);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    strcpy(servaddr.sun_path, UNIXDG_PATH);

    if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        error_handling("bind() error");

    while(1)
    {
        n = recvfrom(sockfd, message, BUF_SIZE, 0, NULL, NULL);
        if (n < 0)
            continue;
        message[n] = 0;
        printf("Receive message: %s\n", message);
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
