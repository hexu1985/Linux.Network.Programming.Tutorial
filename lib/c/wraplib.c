#include "wraplib.h"
#include "error.h"

#include <stddef.h>

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

