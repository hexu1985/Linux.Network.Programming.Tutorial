#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "wrapsock.h"

void client(const char *host, uint16_t port)
{
    int sockfd;
    socklen_t len;
    struct sockaddr_in cliaddr, servaddr;
    char reply[128];

    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0x0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    Inet_pton(AF_INET, host, &servaddr.sin_addr);

    Connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    len = sizeof(cliaddr);
    Getsockname(sockfd, (struct sockaddr *) &cliaddr, &len);
    printf("Client has been assigned socket name (%s)\n", 
            Sock_ntop((struct sockaddr *) &cliaddr, len));

#if 0
    Sendall("Hi there, server");
    Recvall(sockfd, reply, 16);

    printf("The server said %s\n", reply);

    Close(sockfd);
#endif
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
