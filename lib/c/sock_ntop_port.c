#include "sock_ntop_port.h"
#include "error.h"

int
Sock_ntop_port(const struct sockaddr *addr, socklen_t addrlen) 
{
    switch (addr->sa_family) {
    case AF_INET: {
        struct sockaddr_in *sin = (struct sockaddr_in *) addr;
        return ntohs(sin->sin_port);
    }

    default:
        err_msg("Sock_ntop_port: not support AF_xxx: %d, len %d", addr->sa_family, addrlen);
        return 0;
    }
}

