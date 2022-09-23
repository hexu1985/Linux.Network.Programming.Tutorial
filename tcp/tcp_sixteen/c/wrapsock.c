#include "wrapsock.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "error.h"

static char *sock_ntop(const struct sockaddr *addr, socklen_t addrlen);

void
Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    if (connect(sockfd, addr, addrlen) < 0)
        err_sys("connect error");
}

void
Getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    if (getsockname(sockfd, addr, addrlen) < 0)
        err_sys("getsockname error");
}

void
Inet_pton(int af, const char *src, void *dst)
{
    int n;

    if ((n = inet_pton(af, src, dst)) < 0)
        err_sys("inet_pton error for %s", src);  /* errno set */
    else if (n == 0)
        err_quit("inet_pton error for %s", src); /* errno not set */

    /* nothing to return */
}

char *
Sock_ntop(const struct sockaddr *addr, socklen_t addrlen)
{
    char *ptr;

    if ((ptr = sock_ntop(addr, addrlen)) == NULL)
        err_sys("sock_ntop error"); /* inet_ntop() sets errno */
    return(ptr);
}

int 
Socket(int domain, int type, int protocol)
{
    int n;

    if ((n = socket(domain, type, protocol)) < 0)
        err_sys("socket error");
    return(n);
}

static char *sock_ntop(const struct sockaddr *addr, socklen_t addrlen)
{
    char portstr[8];
    static char str[128];

    switch (addr->sa_family) {
        case AF_INET: {
                          struct sockaddr_in *sin = (struct sockaddr_in *) addr;

                          if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
                              return(NULL);
                          if (ntohs(sin->sin_port) != 0) {
                              snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
                              strcat(str, portstr);
                          }
                          return(str);
                      }

        default:
                      snprintf(str, sizeof(str), "sock_ntop: unknown AF_xxx: %d, len %d",
                              addr->sa_family, addrlen);
                      return(str);
    }
    return (NULL);
}
