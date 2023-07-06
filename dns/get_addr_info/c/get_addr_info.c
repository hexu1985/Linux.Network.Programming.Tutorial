#include <stdio.h>

#include "unp.h"

void get_addr_info_list(const char *remote_host)
{
    struct addrinfo hints, *res, *ressave;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;

    Getaddrinfo(remote_host, "", &hints, &res);
    ressave = res;
    int counter = 0;
    do {
        printf("%-2d: %s\n", counter, Sock_ntop(res->ai_addr, res->ai_addrlen));
        counter++;
    } while ( (res = res->ai_next) != NULL);
    Freeaddrinfo(ressave);
}

int main(int argc, char *argv[])
{
    const char *remote_host = "www.python.org";
    if (argc == 2) {
        remote_host = argv[1];
    }
    get_addr_info_list(remote_host);
}
