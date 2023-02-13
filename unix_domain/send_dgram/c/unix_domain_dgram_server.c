#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define _GUN_SOURCE
#include <getopt.h>

#include "wrapsock.h"

#define MAXLINE 1024

void server(const char *path)
{
    int sockfd, n;
    socklen_t len;
	struct sockaddr_un servaddr, cliaddr, myaddr;
    char text[MAXLINE];

	sockfd = Socket(AF_UNIX, SOCK_DGRAM, 0);

    memset(&servaddr, 0x0, sizeof(servaddr));
    servaddr.sun_family = AF_UNIX;
    strcpy(servaddr.sun_path, path);

	Bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr));

    len = sizeof(myaddr);
    Getsockname(sockfd, (struct sockaddr *) &myaddr, &len);
    printf("Listening at %s\n", Sock_ntop((struct sockaddr *) &myaddr, len));

    while (1) {
        len = sizeof(cliaddr);
        n = Recvfrom(sockfd, text, MAXLINE-1, 0, (struct sockaddr *) &cliaddr, &len);
        text[n] = 0;
        printf("The client at %s says %s\n", Sock_ntop((struct sockaddr *) &cliaddr, len), text);
        snprintf(text, MAXLINE, "Your data was %d bytes long", n);
        Sendto(sockfd, text, strlen(text), 0, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
    }
}

#define DEFAULT_PATH    "/tmp/unix_domain_socket_server"

void print_usage(const char *prog) 
{
    printf("Usage: %s [--help] [--path PATH]\n", prog);
    puts("");
    puts("Send and receive UNIX domain datagram locally");
    puts("");
    puts("optional arguments:");
    puts("\t-h, --help    show this help message and exit");
    printf("\t--path PATH   unix-domain address the server listens at (default %s)\n", DEFAULT_PATH);
}

void parse_arguments(int argc, char *argv[], const char **path)
{
    struct option long_options[] = {
        {"path",    required_argument, 0,  0 },
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
            if (strcmp(option_name, "path") == 0)
                *path = optarg;
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
    const char* path = DEFAULT_PATH;

    parse_arguments(argc, argv, &path);

    server(path);

    return 0;
}

