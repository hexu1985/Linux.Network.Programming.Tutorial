#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define _GUN_SOURCE
#include <getopt.h>

#include "unp.h"

void server(const char *interface, uint16_t port)
{
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    char buff[128];
    time_t ticks;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, 0x0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    if (interface && interface[0] != '\0')
        Inet_pton(AF_INET, interface, &servaddr.sin_addr);

    Bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));

    Listen(listenfd, 5);

    for ( ; ; ) {
        connfd = Accept(listenfd, (struct sockaddr *) NULL, NULL);

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Send(connfd, buff, strlen(buff), 0);

        Close(connfd);
    }

    Close(listenfd);
}

#define DEFAULT_HOST    "0.0.0.0"
#define DEFAULT_PORT    1060

void print_usage(const char *prog) 
{
    printf("Usage: %s [--help] [--host HOST] [--port PORT]\n", prog);
    puts("");
    puts("Send and receive over TCP");
    puts("");
    puts("optional arguments:");
    puts("\t-h, --help    show this help message and exit");
    printf("\t--host HOST   IP address the server listens at (default %s)\n", DEFAULT_HOST);
    printf("\t--port PORT   TCP port number (default %d)\n", DEFAULT_PORT);
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
            const char *option_name = long_options[option_index].name;
            if (strcmp(option_name, "host") == 0)
                *host = optarg;
            else if (strcmp(option_name, "port") == 0)
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
    const char *host = DEFAULT_HOST;
    uint16_t port = DEFAULT_PORT;

    parse_arguments(argc, argv, &host, &port);

    server(host, port);

    return 0;
}

