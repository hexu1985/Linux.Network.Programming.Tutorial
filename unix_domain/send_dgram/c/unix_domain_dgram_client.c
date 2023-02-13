#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include <sys/types.h>
#include <unistd.h>

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

const char *mypath()
{
    static char buf[255];
    memset(buf, 0, sizeof(buf));
    snprintf(buf, sizeof(buf), "/tmp/c_unix_socket_client.%d", (int) getpid());
    return buf;
}

void client(const char *path)
{
    int sockfd, n;
    socklen_t len;
    struct sockaddr_un cliaddr, servaddr, myaddr, peeraddr;
    char text[MAXLINE];

    sockfd = Socket(AF_UNIX, SOCK_DGRAM, 0);

    unlink(mypath());
    memset(&cliaddr, 0x0, sizeof(cliaddr));
    cliaddr.sun_family = AF_UNIX;
    strcpy(cliaddr.sun_path, mypath());

    Bind(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr));

    snprintf(text, MAXLINE, "The time is %s", datetime_now());

    memset(&servaddr, 0x0, sizeof(servaddr));
    servaddr.sun_family = AF_UNIX;
    strcpy(servaddr.sun_path, path);

    Sendto(sockfd, text, strlen(text), 0, (struct sockaddr *) &servaddr, sizeof(servaddr));

    len = sizeof(myaddr);
    Getsockname(sockfd, (struct sockaddr *) &myaddr, &len);
    printf("The OS assigned me the address %s\n", Sock_ntop((struct sockaddr *) &myaddr, len));

    len = sizeof(peeraddr);
    n = Recvfrom(sockfd, text, MAXLINE-1, 0, (struct sockaddr *) &peeraddr, &len);
    text[n] = 0;    /* null terminate */
    printf("The server %s replied %s\n", Sock_ntop((struct sockaddr *) &peeraddr, len), text);

    unlink(mypath());

    Close(sockfd);
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
    printf("\t--path PATH   unix-domain address the client sends to (default %s)\n", DEFAULT_PATH);
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

    client(path);

    return 0;
}

