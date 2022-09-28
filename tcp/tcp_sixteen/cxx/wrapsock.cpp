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
SocketAddress::SocketAddress(int family, uint16_t port) {
    switch (family) {
    case AF_INET: {
        auto sin = new struct sockaddr_in;
        memset(sin, 0x0, sizeof(*sin));
        sin->sin_family = AF_INET;
        sin->sin_port = htons(port);
        sin->sin_addr.s_addr = htonl(INADDR_ANY);

        addr = reinterpret_cast<struct sockaddr*>(sin);
        addrlen = sizeof(*sin);
        break;
    }
    default:
        ThrowRuntimeError("SocketAddress(%d) error: unsupport family type", family);
    }
}

SocketAddress::SocketAddress(int family, const char* host, uint16_t port) {
    switch (family) {
    case AF_INET: {
        if (host == nullptr || host[0] == '\0') host = "0.0.0.0";

        auto sin = new struct sockaddr_in;
        memset(sin, 0x0, sizeof(*sin));
        sin->sin_family = AF_INET;
        sin->sin_port = htons(port);

        int n = inet_pton(AF_INET, host, &sin->sin_addr); 
        if (n < 0) {
            ThrowSystemError("SocketAddress(%d, %s, %d), inet_pton error", family, host, port);
        }
        if (n == 0) {
            ThrowRuntimeError("SocketAddress(%d, %s, %d), inet_pton error: Not in presentation format", family, host, port);
        }

        addr = reinterpret_cast<struct sockaddr*>(sin);
        addrlen = sizeof(*sin);
        break;
    }
    default:
        ThrowRuntimeError("SocketAddress(%d, %s, %d) error: unsupport family type", family, host, port);
    }
}

SocketAddress::~SocketAddress() {
    if (addr == nullptr)
        return;

    switch (addr->sa_family) {
    case AF_INET: {
        auto sin = reinterpret_cast<struct sockaddr_in*>(addr);
        delete sin;
        break;
    }
    default:
        assert(false && "invalid family type");
        delete addr;
    }
}

SocketAddress::SocketAddress(SocketAddress&& x) {
    addr = x.addr;
    addrlen = x.addrlen;
    x.addr = nullptr;
    x.addrlen = 0;
}

SocketAddress& SocketAddress::operator=(SocketAddress&& x) {
    using std::swap;
    swap(addr, x.addr);
    swap(addrlen, x.addrlen);

    return *this;
}

std::string SocketAddress::ToString() const {
    if (addr == nullptr)
        ThrowRuntimeError("ToString() error: addr is nullptr", "");

    switch (addr->sa_family) {
    case AF_INET: {
        struct sockaddr_in *sin = (struct sockaddr_in *) addr;

        char str[128];
        if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL) {
            ThrowSystemError("ToString() error: AF_xxx: %d, len %d", addr->sa_family, addrlen);
        }
        std::ostringstream os;
        os << "('" << str << "', " << ntohs(sin->sin_port) << ")";
        return os.str();
    }

    default:
        PrintRuntimeError("ToString() error: unknown AF_xxx: %d, len %d", addr->sa_family, addrlen);
        return "unknown";
    }
}

std::ostream& operator<<(std::ostream& out, const SocketAddress& sock_addr) {
    out << sock_addr.ToString();
    return out;
}

// ======
// Socket
// ======
//
Socket::Socket(int family_, int type, int protocol): family(family_) {
    sockfd = socket(family, type, protocol);
    if (sockfd < 0) 
        ThrowSystemError("Socket(%d, %d, %d) error", family, type, protocol);
}

Socket::Socket() {
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
                ThrowSystemError("Accept() error");
    }
    Socket sock;
    sock.family = family;
    sock.sockfd = n;
    return std::tuple<Socket, SocketAddress>(std::move(sock), std::move(sock_addr));
}

void Socket::Bind(const char* host, uint16_t port) {
    SocketAddress sock_addr(family, host, port);
    Bind(sock_addr);
}

void Socket::Bind(const SocketAddress &sock_addr) {
    auto addr = sock_addr.GetAddrPtr();
    auto addrlen = *sock_addr.GetAddrLenPtr();
    if (bind(sockfd, addr, addrlen) < 0) {
        auto addr_str = sock_addr.ToString();
        ThrowSystemError("Bind(%s) error", addr_str.c_str());
    }
}

void Socket::Close() {
    if (close(sockfd) < 0)
        ThrowSystemError("Close() error");
    sockfd = -1;
}

void Socket::Connect(const char* host, uint16_t port) {
    SocketAddress sock_addr(family, host, port);
    Connect(sock_addr);
}

void Socket::Connect(const SocketAddress &sock_addr) {
    auto addr = sock_addr.GetAddrPtr();
    auto addrlen = *sock_addr.GetAddrLenPtr();
    if (connect(sockfd, addr, addrlen) < 0) {
        auto addr_str = sock_addr.ToString();
        ThrowSystemError("Connect(%s) error", addr_str.c_str());
    }
}

SocketAddress Socket::Getsockname() {
    SocketAddress sock_addr(family);
    auto addr = sock_addr.GetAddrPtr();
    auto addrlen = sock_addr.GetAddrLenPtr();
    if (getsockname(sockfd, addr, addrlen) < 0) {
        ThrowSystemError("Getsockname() error");
    }
    return sock_addr;
}

void Socket::Listen(int backlog) {
    if (listen(sockfd, backlog) < 0) {
        ThrowSystemError("Listen() error");
    }
}

int Socket::Recv(void *buf, size_t len, int flags, std::error_code& ec) {
    ssize_t n = recv(sockfd, buf, len, flags);
    if (n < 0) {
        ec.assign(errno, std::system_category());
    }
    return n;
}

std::string Socket::Recv(size_t len, int flags) {
    std::string buf;
    buf.resize(len);
    std::error_code ec;
    int n = Recv(buf.data(), buf.length(), flags, ec);
    if (ec) {
        ThrowSystemErrorWithCode(ec, "Recv() error");
    }
    buf.resize(n);
    return buf;
}

int Socket::Send(const void *buf, size_t len, int flags, std::error_code& ec) {
    ssize_t n = send(sockfd, buf, len, flags);
    if (n < 0) {
        ec.assign(errno, std::system_category());
    }
    return n;
}

void Socket::Send(const std::string& buf, int flags) {
    std::error_code ec;
    int n = Send(buf.data(), buf.length(), flags, ec);
    if (ec) {
        ThrowSystemErrorWithCode(ec, "Send() error");
    }
    if (n != buf.length()) {
        ThrowRuntimeError("Send() error: uncompleted send, "
                "%d bytes expected, %d bytes in fact",
                n, buf.length());
    }
}

int Socket::SendAll(const void *buf, size_t len, int flags, std::error_code& ec) {
    auto ptr = static_cast<const char*>(buf);
    auto nleft = len;
    ec.clear();
    while (nleft > 0) {
        auto n = Send(ptr, nleft, flags, ec);
        if (ec) {
            if (ec == std::errc::interrupted) {
                ec.clear();
                n = 0;
            }
            return len-nleft;
        }

        nleft -= n;
        ptr += n;
    }
    return len;
}

void Socket::SendAll(const std::string& buf, int flags) {
    std::error_code ec;
    int n = SendAll(buf.data(), buf.length(), flags, ec);
    if (ec) {
        ThrowSystemErrorWithCode(ec, "Send() error");
    }
    assert(n == buf.length());
}

