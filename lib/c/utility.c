#include "utility.h"
#include "error.h"

#include <string.h>
#include <stdio.h>

static const char *sock_ntop(const struct sockaddr *addr, socklen_t addrlen);

void
Gethostname(char *name, size_t len)
{
    if (gethostname(name, len) < 0)
        err_sys("gethostname error");
}

void
Inet_pton(int family, const char *strptr, void *addrptr)
{
    int n;

    if ((n = inet_pton(family, strptr, addrptr)) < 0)
        err_sys("inet_pton error for %s", strptr);  /* errno set */
    else if (n == 0)
        err_quit("inet_pton error for %s", strptr); /* errno not set */

    /* nothing to return */
}

const char *
Inet_ntop(int family, const void *addrptr, char *strptr, socklen_t len)
{
    const char  *ptr;

    if (strptr == NULL)     /* check for old code */
        err_quit("NULL 3rd argument to inet_ntop");
    if ( (ptr = inet_ntop(family, addrptr, strptr, len)) == NULL)
        err_sys("inet_ntop error");     /* sets errno */
    return(ptr);
}

uint16_t
Sock_port(const struct sockaddr *addr, socklen_t addrlen) 
{
    switch (addr->sa_family) {
    case AF_INET: {
        struct sockaddr_in *sin = (struct sockaddr_in *) addr;
        return ntohs(sin->sin_port);
    }

    default:
        err_msg("Sock_port: not support AF_xxx: %d, len %d", addr->sa_family, addrlen);
        return 0;
    }
}

const char *
Sock_ntop(const struct sockaddr *addr, socklen_t addrlen)
{
    const char *ptr;

    if ((ptr = sock_ntop(addr, addrlen)) == NULL)
        err_sys("sock_ntop error"); /* inet_ntop() sets errno */
    return(ptr);
}

static const char *sock_ntop(const struct sockaddr *addr, socklen_t addrlen)
{
    char        portstr[8];
    static char str[128];       /* Unix domain is largest */

    switch (addr->sa_family) {
    case AF_INET: {
        struct sockaddr_in  *sin = (struct sockaddr_in *) addr;

        if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
            return(NULL);
        if (ntohs(sin->sin_port) != 0) {
            snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
            strcat(str, portstr);
        }
        return(str);
    }

    case AF_INET6: {
        struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *) addr;

        str[0] = '[';
        if (inet_ntop(AF_INET6, &sin6->sin6_addr, str + 1, sizeof(str) - 1) == NULL)
            return(NULL);
        if (ntohs(sin6->sin6_port) != 0) {
            snprintf(portstr, sizeof(portstr), "]:%d", ntohs(sin6->sin6_port));
            strcat(str, portstr);
            return(str);
        }
        return (str + 1);
    }

	case AF_UNIX: {
		struct sockaddr_un *unp = (struct sockaddr_un *) addr;

			/* OK to have no pathname bound to the socket: happens on
			   every connect() unless client calls bind() first. */
		if (unp->sun_path[0] == 0)
			strcpy(str, "(no pathname bound)");
		else
			snprintf(str, sizeof(str), "%s", unp->sun_path);
		return(str);
	}

    default:
        snprintf(str, sizeof(str), "sock_ntop: unknown AF_xxx: %d, len %d",
                addr->sa_family, addrlen);
        return(str);
    }
    return (NULL);
}

