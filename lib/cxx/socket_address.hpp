#pragma once

#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <linux/netlink.h>

#include <cstdint>
#include <string>
#include <memory>

template <int I>
struct AddressFamily {
    static constexpr int family = I;
};

class SocketAddress {
public:
    SocketAddress() = default;
    ~SocketAddress() = default;

    SocketAddress(int family);
    SocketAddress(const char* host, uint16_t port, AddressFamily<AF_INET> family=AddressFamily<AF_INET>{});
    SocketAddress(const char* path, AddressFamily<AF_UNIX> family=AddressFamily<AF_UNIX>{});
    SocketAddress(uint32_t groups, uint32_t pid, AddressFamily<AF_NETLINK> family=AddressFamily<AF_NETLINK>{});

    SocketAddress(const SocketAddress&) = delete;
    SocketAddress& operator=(const SocketAddress&) = delete;

    SocketAddress(SocketAddress&& x) = default;
    SocketAddress& operator=(SocketAddress&& x) = default;

    bool SetIPv4(const char* host, uint16_t port);
    bool SetUNIX(const char* path);
    bool SetNetlink(uint32_t groups, uint32_t pid);

    struct sockaddr* GetAddrPtr() { return reinterpret_cast<sockaddr*>(addr.get()); }
    const struct sockaddr* GetAddrPtr() const { return reinterpret_cast<const sockaddr*>(addr.get()); }

    socklen_t* GetAddrLenPtr() { return &addrlen; }
    const socklen_t* GetAddrLenPtr() const { return &addrlen; }

    std::string ToString() const;

private:
    std::unique_ptr<char[]> addr;
    socklen_t addrlen = 0;
};

std::ostream& operator<<(std::ostream& out, const SocketAddress& sock_addr);
std::string to_string(const SocketAddress& sock_addr);


