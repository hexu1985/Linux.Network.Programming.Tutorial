#include "wrapsock.hpp"

#include <unistd.h>

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
        os << str;
        if (ntohs(sin->sin_port) != 0) {
            os << ":" << ntohs(sin->sin_port);
        }
        return os.str();
    }

    default:
        PrintRuntimeError("ToString() error: unknown AF_xxx: %d, len %d", addr->sa_family, addrlen);
        return "unknown";
    }
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

Socket::~Socket() {
    if (sockfd >= 0) {
        close(sockfd);
    }
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
        ThrowSystemError("Getsockname error");
    }
    return sock_addr;
}

