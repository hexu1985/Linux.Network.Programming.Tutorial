#include "wrapsock.h"
#include "error.h"

#include <unistd.h>

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

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

int 
Socket(int family, int type, int protocol)
{
    int n;

    if ((n = socket(family, type, protocol)) < 0)
        err_sys("socket error");
    return(n);
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

