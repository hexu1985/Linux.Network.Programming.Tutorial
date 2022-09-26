#include "wrapsock.hpp"

#include <string.h>
#include <utility>

#include "error.hpp"


// =============
// SocketAddress
// =============
//

SocketAddress::SocketAddress(int family, const char* host, uint16_t port) {
    switch (family) {
    case AF_INET: {
        auto addr_in = new struct sockaddr_in;
        memset(addr_in, 0x, sizeof(*addr_in));
        addr_in.sin_family = AF_INET;
        addr_in.sin_port = htons(port);
        int n = inet_pton(AF_INET, host, &addr_in.sin_addr); 
        if (n < 0) ThrowSystemError("SocketAddress(%d, %s, %d), inet_pton error",family, host, port);
        if (n == 0) ThrowRuntimeError("SocketAddress(%d, %s, %d), inet_pton error: Not in presentation format", family, host, port);

        addr = static_cast<struct sockaddr*>(addr_in);
        addrlen = sizeof(*addr_in);
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
        auto addr_in = static_cast<struct sockaddr_in*>(addr);
        delete addr_in;
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
}

