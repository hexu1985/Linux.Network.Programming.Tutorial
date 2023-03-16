#include <stdio.h>

#include "wrapsock.h"
#include "wrapnetdb.h"

void print_machine_info()
{
    char host_name[128] = {0};
    Gethostname(host_name, sizeof(host_name));
    printf("Host name: %s\n", host_name);

    struct hostent *hptr;
    hptr = Gethostbyname(host_name);
    int addrtype = hptr->h_addrtype;
    assert(addrtype == AF_INET);
    char buf[INET_ADDRSTRLEN];
    for (char **pptr = hptr->h_addr_list; *pptr != NULL; pptr++) {
        printf("IP address: %s\n", Inet_ntop(addrtype, *pptr, buf, sizeof(buf)));
    }
}

int main()
{
    print_machine_info();
}
