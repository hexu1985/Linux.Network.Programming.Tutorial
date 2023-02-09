#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define _GUN_SOURCE
#include <getopt.h>

#include "wrapsock.h"

#define MAXLINE 1024

char *datetime_now()
{
    static char buf[255];
    memset(buf, 0, sizeof(buf));
    time_t t = time(0);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&t));
    return buf;
}

void client(uint16_t port)
{
    int sockfd, n;
    socklen_t len;
    struct sockaddr_in servaddr, myaddr, peeraddr;
    char text[MAXLINE];

    sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

    snprintf(text, MAXLINE, "The time is %s", datetime_now());

    memset(&servaddr, 0x0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    Inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

    Sendto(sockfd, text, strlen(text), 0, (struct sockaddr *) &servaddr, sizeof(servaddr));

    len = sizeof(myaddr);
    Getsockname(sockfd, (struct sockaddr *) &myaddr, &len);
    printf("The OS assigned me the address %s\n", Sock_ntop((struct sockaddr *) &myaddr, len));

    len = sizeof(peeraddr);
    n = Recvfrom(sockfd, text, MAXLINE-1, 0, (struct sockaddr *) &peeraddr, &len);
    text[n] = 0;    /* null terminate */
    printf("The server %s replied %s\n", Sock_ntop((struct sockaddr *) &peeraddr, len), text);

    Close(sockfd);
}

#define DEFAULT_PORT    1060

void print_usage(const char *prog) 
{
    printf("Usage: %s [--help] [--port PORT]\n", prog);
    puts("");
    puts("Send and receive UDP locally");
    puts("");
    puts("optional arguments:");
    puts("\t-h, --help    show this help message and exit");
    printf("\t--port PORT   UDP port (default %d)\n", DEFAULT_PORT);
}

void parse_arguments(int argc, char *argv[], uint16_t *port)
{
    struct option long_options[] = {
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
            const char *option_name = long_options[option_index].name;
            if (strcmp(option_name, "port") == 0)
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
    uint16_t port = DEFAULT_PORT;

    parse_arguments(argc, argv, &port);

    client(port);

    return 0;
}
