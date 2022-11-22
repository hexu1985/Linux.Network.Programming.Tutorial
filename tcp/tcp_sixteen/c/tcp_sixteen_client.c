#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define _GUN_SOURCE
#include <getopt.h>

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

void print_usage(const char *prog) 
{
    printf("Usage: %s [--help] [--host HOST] [--port PORT]\n", prog);
    puts("");
    puts("Send and receive over TCP");
    puts("");
    puts("optional arguments:");
    puts("\t-h, --help            show this help message and exit");
    puts("\t--host HOST           host the client sends to");
    puts("\t--port PORT, -p PORT  TCP port (default 1060)");
}

void parse_arguments(int argc, char *argv[], const char **host, uint16_t *port)
{
    struct option long_options[] = {
        {"host",    required_argument, 0,  0 },
        {"port",    required_argument, 0,  0 },
        {"help",    no_argument,       0, 'h'},
        {0,         0,                 0,  0 }
    };

    int c;
    while (1) {
        int option_index = 0;
        c = getopt_long(argc, argv, "h", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
        case 0:
            printf("option %s", long_options[option_index].name);
            if (optarg)
                printf(" with arg %s", optarg);
            printf("\n");
            if (long_options[option_index].name[0] == 'h')
                *host = optarg;
            else if (long_options[option_index].name[0] == 'p')
                *port = atoi(optarg);
            break;
        case 'h':
        case '?':
            print_usage(argv[0]);
            exit(1);
        }
    }

}

int main(int argc, char *argv[])
{
    const char *host = "127.0.0.1";
    uint16_t port = 1060;

    parse_arguments(argc, argv, &host, &port);

    client(host, port);

    return 0;
}
