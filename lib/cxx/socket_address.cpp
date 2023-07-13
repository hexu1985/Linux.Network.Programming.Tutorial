#include "socket_address.hpp"
#include "error.hpp"
#include "wraplib.hpp"
#include "sock_ntop.hpp"

#include <unistd.h>
#include <errno.h>

#include <cstdlib>
#include <cstring>
#include <cassert>

#include <utility>
#include <sstream>

SocketAddress::SocketAddress(int family) {
    switch (family) {
    case AF_INET:
        addr.reset(malloc_helper<char, struct sockaddr_in>());
        addrlen = sizeof(struct sockaddr_in);
        break;

#ifdef	AF_UNIX
    case AF_UNIX:
        addr.reset(malloc_helper<char, struct sockaddr_un>());
        addrlen = sizeof(struct sockaddr_un);
        break;
#endif

    default:
        ThrowRuntimeError("SocketAddress({}) error: unsupport family type", family);
    }
}

SocketAddress::SocketAddress(const struct sockaddr* addr_, socklen_t addrlen_) {
    addr.reset(malloc_helper<char>(addrlen_));
    memcpy(addr.get(), addr_, addrlen_);
    addrlen = addrlen_;
}

SocketAddress::SocketAddress(const char* host, uint16_t port, AddressFamily<AF_INET>) {
    if (SetIPv4(host, port)) {
        return;
    }
    ThrowRuntimeError("SocketAddress('{}', {}) error: SetIPv4 failed!", host, port);
}

SocketAddress::SocketAddress(const char* path, AddressFamily<AF_UNIX>) {
    if (SetUNIX(path)) {
        return;
    }
    ThrowRuntimeError("SocketAddress('{}') error: SetUNIX failed!", path);
}

SocketAddress::SocketAddress(uint32_t groups, uint32_t pid, AddressFamily<AF_NETLINK>) {
    if (SetNetlink(groups, pid)) {
        return;
    }
    ThrowRuntimeError("SocketAddress({}, {}) error: SetNetlink failed!", groups, pid);
}

bool SocketAddress::SetIPv4(const char* host, uint16_t port) {
    if (host == nullptr || host[0] == '\0') host = "0.0.0.0";

    AddrPtrType<struct sockaddr_in> sin(malloc_helper<struct sockaddr_in>());
    memset(sin.get(), 0x0, sizeof(struct sockaddr_in));
    sin->sin_family = AF_INET;
    sin->sin_port = htons(port);

    int n = inet_pton(AF_INET, host, &sin->sin_addr);
    if (n < 0) {
        PrintSystemError("inet_pton('{}') error", host);
        return false;
    } else if (n == 0) {
        PrintRuntimeError("inet_pton('{}') error: Not in presentation format", host);
        return false;
    }

    addr.reset(reinterpret_cast<char*>(sin.release()));
    addrlen = sizeof(struct sockaddr_in);
    return true;
}

bool SocketAddress::SetUNIX(const char* path) {
    if (path == nullptr || path[0] == '\0') return false;

    AddrPtrType<struct sockaddr_un> sun(malloc_helper<struct sockaddr_un>());
    memset(sun.get(), 0x0, sizeof(struct sockaddr_un));
    sun->sun_family = AF_UNIX;
    if (strlen(path) >= sizeof(sun->sun_path)) {
        PrintRuntimeError("SetUNIX error: path [{}] is too long", path);
        return false;
    }
    strcpy(sun->sun_path, path);

    addr.reset(reinterpret_cast<char*>(sun.release()));
    addrlen = sizeof(struct sockaddr_un);
    return true;
}

bool SocketAddress::SetNetlink(uint32_t groups, uint32_t pid) {
    AddrPtrType<struct sockaddr_nl> snl(malloc_helper<struct sockaddr_nl>());
    memset(snl.get(), 0x0, sizeof(struct sockaddr_nl));
    snl->nl_family = AF_NETLINK;
    snl->nl_groups = groups;
    snl->nl_pid = pid;

    addr.reset(reinterpret_cast<char*>(snl.release()));
    addrlen = sizeof(struct sockaddr_nl);
    return true;
}

std::string SocketAddress::IPAddress() const {
    if (addr == nullptr)
        return "";

    switch (GetAddrPtr()->sa_family) {
    case AF_INET: {
        struct sockaddr_in *sin = (struct sockaddr_in *) GetAddrPtr();
        return Inet_ntop(AF_INET, &sin->sin_addr);
    }

    case AF_INET6: {
        struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *) GetAddrPtr();
        return Inet_ntop(AF_INET6, &sin6->sin6_addr);
    }

    default:
        PrintRuntimeError("SocketAddress::IPAddress: unsupported AF_xxx: {}, len {}", GetAddrPtr()->sa_family, addrlen);
        return "";
    }
    return "";
}

int SocketAddress::IPPort() const {
    if (addr == nullptr)
        return -1;

    switch (GetAddrPtr()->sa_family) {
    case AF_INET: {
        struct sockaddr_in *sin = (struct sockaddr_in *) GetAddrPtr();
        return ntohs(sin->sin_port);
    }

    case AF_INET6: {
        struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *) GetAddrPtr();
        return ntohs(sin6->sin6_port);
    }

    default:
        PrintRuntimeError("SocketAddress::IPPort: unsupported AF_xxx: {}, len {}", GetAddrPtr()->sa_family, addrlen);
        return -1;
    }
    return -1;
}

std::string SocketAddress::ToString() const {
    if (addr == nullptr)
        return "";

    return Sock_ntop((const struct sockaddr *) addr.get(), addrlen);
}

std::ostream& operator<<(std::ostream& out, const SocketAddress& sock_addr) {
    out << sock_addr.ToString();
    return out;
}

std::string to_string(const SocketAddress& sock_addr) {
    return sock_addr.ToString();
}

