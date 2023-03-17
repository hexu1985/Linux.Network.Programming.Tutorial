#include <stdio.h>
#include <assert.h>

#include "wrapsock.h"
#include "wrapnetdb.h"

void get_host_by_name(const char *remote_host)
{
    struct hostent *hptr;
    hptr = Gethostbyname(remote_host);
    int addrtype = hptr->h_addrtype;
    assert(addrtype == AF_INET);
    char buf[INET_ADDRSTRLEN];
    for (char **pptr = hptr->h_addr_list; *pptr != NULL; pptr++) {
        printf("IP address of %s: %s\n", remote_host, Inet_ntop(addrtype, *pptr, buf, sizeof(buf)));
    }
}

int main(int argc, char *argv[])
{
    const char *remote_host = "www.python.org";
    if (argc == 2) {
        remote_host = argv[1];
    }
    get_host_by_name(remote_host);
}
