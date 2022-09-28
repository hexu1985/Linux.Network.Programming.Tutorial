#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "wrapsock.h"

void sendall(int sockfd, const char *str)
{
    Writen(sockfd, str, strlen(str));
}

void recvall(int sockfd, char *buf, int length)
{
    int ntotal = 0;
    int nread = 0;
    char *ptr = buf;

    while (ntotal < length) {
        nread = Recv(sockfd, ptr, length-ntotal, 0);
        if (nread == 0) {
            err_quit("was expecting %d bytes but only received"
                    " %d bytes before the socket closed",
                    length, ntotal);
        }
        ntotal += nread;
        ptr += nread;
    }
}

void client(const char *host, uint16_t port)
{
    int sockfd;
    socklen_t len;
    struct sockaddr_in myaddr, servaddr;
    char reply[128];

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0x0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    Inet_pton(AF_INET, host, &servaddr.sin_addr);

    Connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    len = sizeof(myaddr);
    Getsockname(sockfd, (struct sockaddr *) &myaddr, &len);
    printf("Client has been assigned socket name %s\n", 
            Sock_ntop((struct sockaddr *) &myaddr, len));

    sendall(sockfd, "Hi there, server");
    recvall(sockfd, reply, 16);

    printf("The server said %s\n", reply);

    Close(sockfd);
}

int main(int argc, char *argv[])
{
    const char *host = "127.0.0.1";
    uint16_t port = 1060;

    if (argc > 1) {
        host = argv[1];
    }
    if (argc > 2) {
        port = atoi(argv[2]);
    }

    client(host, port);

    return 0;
}
