#include "wrapsock.h"

#include <unistd.h>

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

static char *sock_ntop(const struct sockaddr *addr, socklen_t addrlen);

int
Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    int n;

again:
    if ((n = accept(sockfd, addr, addrlen)) < 0) {
#ifdef	EPROTO
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

char *
Sock_ntop(const struct sockaddr *addr, socklen_t addrlen)
{
    char *ptr;

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

static char *sock_ntop(const struct sockaddr *addr, socklen_t addrlen)
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

    default:
        snprintf(str, sizeof(str), "sock_ntop: unknown AF_xxx: %d, len %d",
                addr->sa_family, addrlen);
        return(str);
    }
    return (NULL);
}

void SendAll(int fd, const void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;

	ptr = (const char *) vptr;
	nleft = n;
	while (nleft > 0) {
		if ((nwritten = send(fd, ptr, nleft, 0)) <= 0) {
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;
			else
                err_sys("send error");
		}

		nleft -= nwritten;
		ptr   += nwritten;
	}
}

void SendMsg(int fd, const char *msg)
{
    SendAll(fd, msg, strlen(msg));
}

void RecvAll(int fd, void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nread;
	const char *ptr;

	ptr = (char *) vptr;
	nleft = n;
	while (nleft > 0) {
		if ((nread = recv(fd, ptr, nleft, 0)) <= 0) {
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

