#include "wrapsock.hpp"

#include <unistd.h>
#include <errno.h>

#include <cstring>
#include <cassert>

#include <utility>
#include <sstream>

#include "error.hpp"

// =============
// SocketAddress
// =============
//
SocketAddress::SocketAddress(int family) {
    switch (family) {
    case AF_INET:
        addr.reset(reinterpret_cast<char *>(new struct sockaddr_in));
        addrlen = sizeof(struct sockaddr_in);
        break;

#ifdef	AF_UNIX
    case AF_UNIX:
        addr.reset(reinterpret_cast<char *>(new struct sockaddr_un));
        addrlen = sizeof(struct sockaddr_un);
        break;
#endif

    default:
        ThrowRuntimeError("SocketAddress(%d) error: unsupport family type", family);
    }
}

SocketAddress::SocketAddress(const char* host, uint16_t port, AddressFamily<AF_INET>) {
    if (SetIPv4(host, port)) {
        return;
    }
    ThrowRuntimeError("SocketAddress(%s, %d) error: SetIPv4 failed!", host, port);
}

SocketAddress::SocketAddress(const char* path, AddressFamily<AF_UNIX>) {
    if (SetUNIX(path)) {
        return;
    }
    ThrowRuntimeError("SocketAddress(%s) error: SetUNIX failed!", path);
}

SocketAddress::SocketAddress(uint32_t groups, uint32_t pid, AddressFamily<AF_NETLINK>) {
    if (SetNetlink(groups, pid)) {
        return;
    }
    ThrowRuntimeError("SocketAddress(%u, %u) error: SetNetlink failed!", groups, pid);
}

bool SocketAddress::SetIPv4(const char* host, uint16_t port) {
    if (host == nullptr || host[0] == '\0') host = "0.0.0.0";

    std::unique_ptr<struct sockaddr_in> sin(new struct sockaddr_in);
    memset(sin.get(), 0x0, sizeof(struct sockaddr_in));
    sin->sin_family = AF_INET;
    sin->sin_port = htons(port);

    int n = inet_pton(AF_INET, host, &sin->sin_addr);
    if (n < 0) {
        PrintSystemError("inet_pton(%s) error", host);
        return false;
    } else if (n == 0) {
        PrintRuntimeError("inet_pton(%s) error: Not in presentation format", host);
        return false;
    }

    addr.reset(reinterpret_cast<char*>(sin.release()));
    addrlen = sizeof(struct sockaddr_in);
    return true;
}

bool SocketAddress::SetUNIX(const char* path) {
    if (path == nullptr || path[0] == '\0') return false;

    std::unique_ptr<struct sockaddr_un> sun(new struct sockaddr_un);
    memset(sun.get(), 0x0, sizeof(struct sockaddr_un));
    sun->sun_family = AF_UNIX;
    if (strlen(path) >= sizeof(sun->sun_path)) {
        PrintRuntimeError("SetUNIX error: path [%s] is too long", path);
        return false;
    }
    strcpy(sun->sun_path, path);

    addr.reset(reinterpret_cast<char*>(sun.release()));
    addrlen = sizeof(struct sockaddr_un);
    return true;
}

bool SocketAddress::SetNetlink(uint32_t groups, uint32_t pid) {
    std::unique_ptr<struct sockaddr_nl> snl(new struct sockaddr_nl);
    memset(snl.get(), 0x0, sizeof(struct sockaddr_nl));
    snl->nl_family = AF_NETLINK;
    snl->nl_groups = groups;
    snl->nl_pid = pid;

    addr.reset(reinterpret_cast<char*>(snl.release()));
    addrlen = sizeof(struct sockaddr_nl);
    return true;
}

std::string SocketAddress::ToString() const {
    if (addr == nullptr)
        ThrowRuntimeError("ToString() error: addr is nullptr", "");

    return Sock_ntop((const struct sockaddr *) addr.get(), addrlen);
}

std::ostream& operator<<(std::ostream& out, const SocketAddress& sock_addr) {
    out << sock_addr.ToString();
    return out;
}

std::string to_string(const SocketAddress& sock_addr) {
    return sock_addr.ToString();
}

// ======
// Socket
// ======
//
Socket::Socket(int family_, int type, int protocol): family(family_) {
    sockfd = socket(family, type, protocol);
    if (sockfd < 0) 
        ThrowSystemError("socket(%d, %d, %d) error", family, type, protocol);
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
        ThrowSystemError("bind(%s) error", addr_str.c_str());
    }
}

void Socket::Close() {
    if (close(sockfd) < 0)
        ThrowSystemError("close() error");
    sockfd = -1;
}

void Socket::Connect(const SocketAddress& sock_addr) {
    auto addr = sock_addr.GetAddrPtr();
    auto addrlen = *sock_addr.GetAddrLenPtr();
    if (connect(sockfd, addr, addrlen) < 0) {
        auto addr_str = sock_addr.ToString();
        ThrowSystemError("connect(%s) error", addr_str.c_str());
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
                ThrowRuntimeError("RecvAll() error: was expecting %d bytes but only received"
                                  " %d bytes before the socket closed", len, len-nleft);
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
        ThrowSystemError("shutdown(%d) error", how);
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

// =================
// Utility Functions
// =================
//

std::string Inet_ntop(int family, const void *addrptr, std::error_code& ec) {
    char buf[INET6_ADDRSTRLEN] = {0};
    const char* ptr;
    if ((ptr = inet_ntop(family, addrptr, buf, sizeof(buf))) == NULL) {
        ec.assign(errno, std::system_category());
        return "";
    }
    return ptr;
}

std::string Inet_ntop(int family, const void *addrptr) {
    std::error_code ec;
    auto addr_str = Inet_ntop(family, addrptr, ec);
    if (ec) {
        ThrowSystemErrorWithCode(ec, "Inet_ntop() error");
    }
    return addr_str;
}

std::string Sock_ntop(const struct sockaddr *addr, socklen_t addrlen, std::error_code& ec) {
    switch (addr->sa_family) {
    case AF_INET: {
        struct sockaddr_in  *sin = (struct sockaddr_in *) addr;

        auto addr_str = Inet_ntop(AF_INET, &sin->sin_addr, ec);
        if (addr_str.empty()) {
            return "";
        }
        if (ntohs(sin->sin_port) != 0) {
            return addr_str+":"+std::to_string(ntohs(sin->sin_port));
        }
        return addr_str;
    }

    case AF_INET6: {
        struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *) addr;

        auto addr_str = Inet_ntop(AF_INET6, &sin6->sin6_addr, ec);
        if (addr_str.empty()) {
            return "";
        }
        if (ntohs(sin6->sin6_port) != 0) {
            return "["+addr_str+"]:"+std::to_string(ntohs(sin6->sin6_port));
        }
        return addr_str;
    }

	case AF_UNIX: {
		struct sockaddr_un *unp = (struct sockaddr_un *) addr;

			/* OK to have no pathname bound to the socket: happens on
			   every connect() unless client calls bind() first. */
		if (unp->sun_path[0] == 0)
            return "(no pathname bound)";
		else
            return unp->sun_path;
	}

    default:
        PrintRuntimeError("sock_ntop: unknown AF_xxx: %d, len %d", addr->sa_family, addrlen);
        ec.assign(EAFNOSUPPORT, std::system_category());
        return "";
    }
    return "";
}

std::string Sock_ntop(const struct sockaddr *addr, socklen_t addrlen) {
    std::error_code ec;
    auto addr_str = Sock_ntop(addr, addrlen, ec);
    if (ec) {
        ThrowSystemErrorWithCode(ec, "Sock_ntop() error");
    }
    return addr_str;
}

std::string Gethostname(std::error_code& ec) {
    char host_name[128] = {0};
    if (gethostname(host_name, sizeof(host_name)) < 0) {
        ec.assign(errno, std::system_category());
        return "";
    }
    return host_name;
}

std::string Gethostname() {
    std::error_code ec;
    auto host_name = Gethostname(ec);
    if (ec) {
        ThrowSystemErrorWithCode(ec, "Gethostname() error");
    }
    return host_name;
}
