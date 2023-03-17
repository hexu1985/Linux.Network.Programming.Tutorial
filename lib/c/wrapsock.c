#include "wrapsock.h"

#include <unistd.h>

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

static const char *sock_ntop(const struct sockaddr *addr, socklen_t addrlen);

int
Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    int n;

again:
    if ((n = accept(sockfd, addr, addrlen)) < 0) {
#ifdef EPROTO
        if (errno == EPROTO || errno == ECONNABORTED)
#else
            if (errno == ECONNABORTED)
#endif
                goto again;
            else
                err_sys("accept error");
    }
    return(n);
}

void
Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    if (bind(sockfd, addr, addrlen) < 0)
        err_sys("bind error");
}

void
Close(int fd)
{
    if (close(fd) == -1)
        err_sys("close error");
}

void
Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    if (connect(sockfd, addr, addrlen) < 0)
        err_sys("connect error");
}

void
Getpeername(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    if (getpeername(sockfd, addr, addrlen) < 0)
        err_sys("getpeername error");
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

void
Listen(int sockfd, int backlog)
{
    if (listen(sockfd, backlog) < 0)
        err_sys("listen error");
}

ssize_t 
Recv(int sockfd, void *buf, size_t len, int flags)
{
    ssize_t n;

    if ((n = recv(sockfd, buf, len, flags)) < 0)
        err_sys("recv error");
    return(n);
}

void
Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
{
    if (setsockopt(sockfd, level, optname, optval, optlen) < 0)
        err_sys("setsockopt error");
}

const char *
Sock_ntop(const struct sockaddr *addr, socklen_t addrlen)
{
    const char *ptr;

    if ((ptr = sock_ntop(addr, addrlen)) == NULL)
        err_sys("sock_ntop error"); /* inet_ntop() sets errno */
    return(ptr);
}

int 
Socket(int family, int type, int protocol)
{
    int n;

    if ((n = socket(family, type, protocol)) < 0)
        err_sys("socket error");
    return(n);
}

static const char *sock_ntop(const struct sockaddr *addr, socklen_t addrlen)
{
    char ipstr[128];
    static char str[128];

    switch (addr->sa_family) {
    case AF_INET: {
        struct sockaddr_in *sin = (struct sockaddr_in *) addr;

        if (inet_ntop(AF_INET, &sin->sin_addr, ipstr, sizeof(ipstr)) == NULL)
            return(NULL);
        snprintf(str, sizeof(str), "('%s', %d)", ipstr, ntohs(sin->sin_port));
        return(str);
    }

#ifdef	AF_UNIX
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
#endif

    default:
        snprintf(str, sizeof(str), "sock_ntop: unknown AF_xxx: %d, len %d",
                addr->sa_family, addrlen);
        return(str);
    }
    return (NULL);
}

void Send(int sockfd, const void *buf, size_t len, int flags)
{
    if (send(sockfd, buf, len, flags) != (ssize_t)len)
        err_sys("send error");
}

void SendAll(int sockfd, const void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = (const char *) vptr;
    nleft = n;
    while (nleft > 0) {
        if ((nwritten = send(sockfd, ptr, nleft, 0)) <= 0) {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;
            else
                err_sys("send error");
        }

        nleft -= nwritten;
        ptr   += nwritten;
    }
}

void SendMsg(int sockfd, const char *msg)
{
    SendAll(sockfd, msg, strlen(msg));
}

void RecvAll(int sockfd, void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nread;
    char *ptr;

    ptr = (char *) vptr;
    nleft = n;
    while (nleft > 0) {
        if ((nread = recv(sockfd, ptr, nleft, 0)) <= 0) {
            if (nread == 0)
                err_quit("was expecting %d bytes but only received"
                        " %d bytes before the socket closed", n, n-nleft);
            else if (errno == EINTR)
                nread = 0;
            else
                err_sys("send error");
        }

        nleft -= nread;
        ptr   += nread;
    }
}

void Shutdown(int sockfd, int how)
{
    if (shutdown(sockfd, how) < 0)
        err_sys("shutdown error");
}

void
Sendto(int sockfd, const void *buf, size_t len, int flags,
        const struct sockaddr *dest_addr, socklen_t addrlen)
{
    if (sendto(sockfd, buf, len, flags, dest_addr, addrlen) != (ssize_t)len)
        err_sys("sendto error");
}

ssize_t 
Recvfrom(int sockfd, void *buf, size_t len, int flags,
        struct sockaddr *src_addr, socklen_t *addrlen)
{
    ssize_t n;

	if ((n = recvfrom(sockfd, buf, len, flags, src_addr, addrlen)) < 0)
		err_sys("recvfrom error");
	return(n);
}

void
Gethostname(char *name, size_t len)
{
    if (gethostname(name, len) < 0)
        err_sys("gethostname error");
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
