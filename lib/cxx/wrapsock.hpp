#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <string>
#include <iosfwd>
#include <tuple>

#include "error.hpp"

// =============
// SocketAddress
// =============
//
class SocketAddress {
public:
    SocketAddress(int family, uint16_t port=0);
    SocketAddress(int family, const char* host, uint16_t port);

    ~SocketAddress();

    SocketAddress(const SocketAddress&) = delete;
    SocketAddress& operator=(const SocketAddress&) = delete;

    SocketAddress(SocketAddress&& x);
    SocketAddress& operator=(SocketAddress&& x);

    struct sockaddr* GetAddrPtr() { return addr; }
    const struct sockaddr* GetAddrPtr() const { return addr; }

    socklen_t* GetAddrLenPtr() { return &addrlen; }
    const socklen_t* GetAddrLenPtr() const { return &addrlen; }

    std::string ToString() const;

private:
    sockaddr* addr = nullptr;
    socklen_t addrlen = 0;
};

std::ostream& operator<<(std::ostream& out, const SocketAddress& sock_addr);

// ======
// Socket
// ======
//
class Socket {
public:
    Socket(int family, int type, int protocol=0);

    ~Socket();

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&& x);
    Socket& operator=(Socket&& x);

    std::tuple<Socket, SocketAddress> Accept();

    void Bind(const char* host, uint16_t port);
    void Bind(const SocketAddress &sock_addr);

    void Close();

    void Connect(const char* host, uint16_t port);
    void Connect(const SocketAddress &sock_addr);

    int GetDescriptor() { return sockfd; }
    int GetFamily() { return family; }

    SocketAddress Getpeername();
    SocketAddress Getsockname();

    void Listen(int backlog);

    int Recv(void *buf, size_t len, int flags, std::error_code& ec);
    std::string Recv(size_t len, int flags=0);

    int Send(const void *buf, size_t len, int flags, std::error_code& ec);
    void Send(const std::string& buf, int flags=0);

    int SendAll(const void *buf, size_t len, int flags, std::error_code& ec);
    void SendAll(const std::string& buf, int flags=0);

    template <typename OptValT>
    void Setsockopt(int level, int optname, const OptValT& optval);

private:
    Socket();

private:
    int sockfd = -1;
    int family = -1;
};

template <typename OptValT>
void Socket::Setsockopt(int level, int optname, const OptValT& optval) {
    if (setsockopt(sockfd, level, optname, &optval, sizeof(OptValT)) < 0)
        ThrowSystemError("Setsockopt(%d, %d) error", level, optname);
}
