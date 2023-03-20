#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <netdb.h>

#define _GUN_SOURCE
#include <getopt.h>

#include "unp.h"

void connect_to(const char *host, const char *serv)
{
    int             sockfd, n;
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_ADDRCONFIG | AI_V4MAPPED | AI_CANONNAME;

    if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0)
        err_quit("Name service failure: %s\n", gai_strerror(n));

    sockfd = Socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0) {
        printf("Success: host %s is listening on port %d\n", res->ai_canonname, (int) Sock_port(res->ai_addr, res->ai_addrlen));
    } else {
        err_sys("Network failure");
    }

    close(sockfd);
    freeaddrinfo(res);
    return;
}

#define DEFAULT_SERV    "www"

void print_usage(const char *prog) 
{
    printf("Usage: %s [--help] [--host HOST] [--serv SERV]\n", prog);
    puts("");
    puts("Try connecting by tcp");
    puts("");
    puts("optional arguments:");
    puts("\t-h, --help    show this help message and exit");
    printf("\t--host HOST   hostname that you want to contact\n");
    printf("\t--serv SERV   TCP service name or port number(default %s)\n", DEFAULT_SERV);
}

void parse_arguments(int argc, char *argv[], const char **host, const char **serv)
{
    struct option long_options[] = {
        {"host",    required_argument, 0,  0 },
        {"serv",    required_argument, 0,  0 },
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
            else if (strcmp(option_name, "serv") == 0)
                *serv = optarg;
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
    const char *host = NULL;
    const char *serv = DEFAULT_SERV;

    parse_arguments(argc, argv, &host, &serv);

    if (host == NULL) {
        print_usage(argv[0]);
        exit(1);
    }

    connect_to(host, serv);

    return 0;
}
