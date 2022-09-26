#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <string>

// =============
// SocketAddress
// =============
//
class SocketAddress {
public:
    SocketAddress(int family, const char* host, uint16_t port);

    ~SocketAddress();

    SocketAddress(const SocketAddress&) = delete;
    SocketAddress& operator=(const SocketAddress&) = delete;

    SocketAddress(SocketAddress&& x);
    SocketAddress& operator=(SocketAddress&& x);

    struct sockaddr* GetAddrPtr() { return addr; }
    socklen_t* GetAddrLenPtr() { return &addrlen; }

private:
    sockaddr* addr = nullptr;
    socklen_t addrlen = 0;
};

