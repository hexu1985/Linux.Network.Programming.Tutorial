#include "wrapsock.hpp"
#include "error.hpp"

#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#include <cstring>
#include <cassert>

#include <utility>
#include <sstream>

Socket::Socket(int family_, int type, int protocol): family(family_) {
    sockfd = socket(family, type, protocol);
    if (sockfd < 0) 
        ThrowSystemError("socket({}, {}, {}) error", family, type, protocol);
}

Socket::~Socket() {
    if (sockfd >= 0) {
        close(sockfd);
    }
}

Socket::Socket(Socket&& x) {
    sockfd = x.sockfd;
    family = x.family;
    x.sockfd = -1;
    x.family = -1;
}

Socket& Socket::operator=(Socket&& x) {
    using std::swap;
    swap(sockfd, x.sockfd);
    swap(family, x.family);

    return *this;
}

std::tuple<Socket, SocketAddress> Socket::Accept() {
    SocketAddress sock_addr(family);
    auto addr = sock_addr.GetAddrPtr();
    auto addrlen = sock_addr.GetAddrLenPtr();

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
                ThrowSystemError("accept() error");
    }
    Socket sock;
    sock.family = family;
    sock.sockfd = n;
    return std::tuple<Socket, SocketAddress>(std::move(sock), std::move(sock_addr));
}

void Socket::Bind(const SocketAddress& sock_addr) {
    auto addr = sock_addr.GetAddrPtr();
    auto addrlen = *sock_addr.GetAddrLenPtr();
    if (bind(sockfd, addr, addrlen) < 0) {
        auto addr_str = sock_addr.ToString();
        ThrowSystemError("bind('{}') error", addr_str);
    }
}

void Socket::Close() {
    if (close(sockfd) < 0)
        ThrowSystemError("close() error");
    sockfd = -1;
}

void Socket::Connect(const SocketAddress& sock_addr, std::error_code& ec) {
    auto addr = sock_addr.GetAddrPtr();
    auto addrlen = *sock_addr.GetAddrLenPtr();
    if (connect(sockfd, addr, addrlen) < 0) {
        ec.assign(errno, std::system_category());
    }
}

void Socket::Connect(const SocketAddress& sock_addr) {
    std::error_code ec;
    Connect(sock_addr, ec);
    if (ec) {
        auto addr_str = sock_addr.ToString();
        ThrowSystemErrorWithCode(ec, "connect('{}') error", addr_str);
    }
}

SocketAddress Socket::Getpeername() {
    SocketAddress sock_addr(family);
    auto addr = sock_addr.GetAddrPtr();
    auto addrlen = sock_addr.GetAddrLenPtr();
    if (getpeername(sockfd, addr, addrlen) < 0) {
        ThrowSystemError("getpeername() error");
    }
    return sock_addr;
}

SocketAddress Socket::Getsockname() {
    SocketAddress sock_addr(family);
    auto addr = sock_addr.GetAddrPtr();
    auto addrlen = sock_addr.GetAddrLenPtr();
    if (getsockname(sockfd, addr, addrlen) < 0) {
        ThrowSystemError("getsockname() error");
    }
    return sock_addr;
}

void Socket::Listen(int backlog) {
    if (listen(sockfd, backlog) < 0) {
        ThrowSystemError("listen() error");
    }
}

int Socket::Send(const void* buf, size_t len, int flags, std::error_code& ec) {
    ssize_t n = send(sockfd, buf, len, flags);
    if (n < 0) {
        ec.assign(errno, std::system_category());
    }
    return n;
}

void Socket::Send(const void* buf, size_t len) {
    std::error_code ec;
    if (Send(buf, len, 0, ec) != (ssize_t) len) {
        ThrowSystemErrorWithCode(ec, "Send() error");
    }
}

void Socket::Send(const std::string& buf) {
    Send(buf.data(), buf.size());
}

void Socket::SendAll(const void* buf, size_t len) {
    auto ptr = static_cast<const char*>(buf);
    auto nleft = len;
    auto nwritten = 0;
    while (nleft > 0) {
        std::error_code ec;
        if ((nwritten = Send(ptr, nleft, 0, ec)) <= 0) {
            if (nwritten < 0 && ec == std::errc::interrupted) {
                nwritten = 0;
            } else {
                ThrowSystemErrorWithCode(ec, "Send() error");
            }
        }

        nleft -= nwritten;
        ptr += nwritten;
    }
}

void Socket::SendAll(const std::string& buf) {
    SendAll(buf.data(), buf.size());
}

int Socket::Recv(void* buf, size_t len, int flags, std::error_code& ec) {
    ssize_t n = recv(sockfd, buf, len, flags);
    if (n < 0) {
        ec.assign(errno, std::system_category());
    }
    return n;
}

int Socket::Recv(void* buf, size_t len) {
    std::error_code ec;
    auto n = Recv(buf, len, 0, ec);
    if (ec) {
        ThrowSystemErrorWithCode(ec, "Recv() error");
    }
    return n;
}

std::string Socket::Recv(size_t len) {
    std::string buf;
    buf.resize(len);
    auto n = Recv((void *) buf.data(), buf.size());
    buf.resize(n);
    return buf;
}

void Socket::RecvAll(void* buf, size_t len) {
    auto ptr = static_cast<char*>(buf);
    auto nleft = len;
    auto nread = 0;
    while (nleft > 0) {
        std::error_code ec;
        if ((nread = Recv(ptr, nleft, 0, ec)) <= 0) {
            if (nread == 0) {
                ThrowRuntimeError("RecvAll() error: was expecting {} bytes but only received"
                                  " {} bytes before the socket closed", len, len-nleft);
            } else if(ec == std::errc::interrupted) {
                nread = 0;
            } else {
                ThrowSystemErrorWithCode(ec, "Recv() error");
            }
        }

        nleft -= nread;
        ptr += nread;
    }
}

std::string Socket::RecvAll(size_t len) {
    std::string buf;
    buf.resize(len);
    RecvAll((void *) buf.data(), buf.size());
    return buf;
}

void Socket::Shutdown(int how) {
    if (shutdown(sockfd, how) < 0) {
        ThrowSystemError("shutdown({}) error", how);
    }
}

int Socket::SendTo(const void* buf, size_t len, int flags, const SocketAddress& dest_addr, std::error_code& ec) {
    auto addr = dest_addr.GetAddrPtr();
    auto addrlen = *dest_addr.GetAddrLenPtr();
    ssize_t n = sendto(sockfd, buf, len, flags, addr, addrlen);
    if (n < 0) {
        ec.assign(errno, std::system_category());
    }
    return n;
}

void Socket::SendTo(const void* buf, size_t len, const SocketAddress& dest_addr) {
    std::error_code ec;
    if (SendTo(buf, len, 0, dest_addr, ec) != (ssize_t) len) {
        ThrowSystemErrorWithCode(ec, "SendTo() error");
    }
}

void Socket::SendTo(const std::string& buf, const SocketAddress& dest_addr) {
    SendTo(buf.data(), buf.size(), dest_addr);
}

int Socket::RecvFrom(void* buf, size_t len, int flags, SocketAddress& src_addr, std::error_code& ec) {
    auto addr = src_addr.GetAddrPtr();
    auto addrlen = src_addr.GetAddrLenPtr();
    ssize_t n = recvfrom(sockfd, buf, len, flags, addr, addrlen);
    if (n < 0) {
        ec.assign(errno, std::system_category());
    }
    return n;
}

int Socket::RecvFrom(void* buf, size_t len, SocketAddress& src_addr) {
    std::error_code ec;
    auto n = RecvFrom(buf, len, 0, src_addr, ec);
    if (ec) {
        ThrowSystemErrorWithCode(ec, "RecvFrom() error");
    }
    return n;
}

std::tuple<std::string, SocketAddress> Socket::RecvFrom(size_t len) {
    std::string buf;
    buf.resize(len);
    SocketAddress src_addr(family);
    auto n = RecvFrom((void *) buf.data(), buf.size(), src_addr);
    buf.resize(n);
    return std::tuple<std::string, SocketAddress>(std::move(buf), std::move(src_addr));
}

