#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "wrapsock.h"

void Sendall(int sockfd, const char *str)
{
    Writen(sockfd, str, strlen(str));
}

void Recvall(int sockfd, char *buf, int n)
{
    Readn(sockfd, buf, n);
}

void server(const char *interface, uint16_t port)
{
    int listenfd, connfd;
    socklen_t len;
    struct sockaddr_in myaddr, peeraddr, cliaddr, servaddr;
    const int on = 1;
    char message[128];

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    memset(&servaddr, 0x0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    if (interface && interface[0] != '\0')
        Inet_pton(AF_INET, interface, &servaddr.sin_addr);

    Bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));

    Listen(listenfd, 1);

    len = sizeof(myaddr);
    Getsockname(listenfd, (struct sockaddr *) &myaddr, &len);
    printf("Client has been assigned socket name (%s)\n", 
            Sock_ntop((struct sockaddr *) &myaddr, len));

    while (1) {
        printf("Waiting to accept a new connection");
        len = sizeof(cliaddr);
        connfd = Accept(listenfd, (struct sockaddr *) &cliaddr, &len);
        printf("We have accepted a connection from (%s)\n", 
                Sock_ntop((struct sockaddr *) &cliaddr, len));

        len = sizeof(myaddr);
        Getsockname(connfd, (struct sockaddr *) &myaddr, &len);
        printf("  Socket name: (%s)\n", Sock_ntop((struct sockaddr *) &myaddr, len));

        len = sizeof(peeraddr);
        Getpeername(connfd, (struct sockaddr *) &peeraddr, &len);
        printf("  Socket peer: (%s)\n", Sock_ntop((struct sockaddr *) &peeraddr, len));

        Recvall(connfd, message, 16);
        printf("  Incoming sixteen-octet message: %s\n", message);
        Sendall(connfd, "Farewell, client\n");
        Close(connfd);
        printf("  Reply sent, socket closed\n");
    }

    Close(listenfd);
}

int main(int argc, char *argv[])
{
    const char *host = "";
    uint16_t port = 1060;

    if (argc > 1) {
        host = argv[1];
    }
    if (argc > 2) {
        port = atoi(argv[2]);
    }

    server(host, port);

    return 0;
}

