#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>

#include <string>
#include <iosfwd>
#include <tuple>
#include <memory>

#include "error.hpp"

template <int I>
struct AddressFamily {
    static constexpr int family = I;
};

// =============
// SocketAddress
// =============
//
class SocketAddress {
public:
    SocketAddress() = default;
    ~SocketAddress() = default;

    SocketAddress(int family);
    SocketAddress(const char* host, uint16_t port, AddressFamily<AF_INET> family=AddressFamily<AF_INET>{});
    SocketAddress(const char* path, AddressFamily<AF_UNIX> family=AddressFamily<AF_UNIX>{});
    SocketAddress();

    SocketAddress(const SocketAddress&) = delete;
    SocketAddress& operator=(const SocketAddress&) = delete;

    SocketAddress(SocketAddress&& x) = default;
    SocketAddress& operator=(SocketAddress&& x) = default;

    bool SetIPv4(const char* host, uint16_t port);
    bool SetUNIX(const char* path);

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


// ======
// Socket
// ======
//
class Socket {
public:
    Socket() = default;
    Socket(int family, int type, int protocol=0);

    ~Socket();

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&& x);
    Socket& operator=(Socket&& x);

    std::tuple<Socket, SocketAddress> Accept();

    void Bind(const SocketAddress& sock_addr);

    void Close();

    void Connect(const SocketAddress& sock_addr);

    int GetDescriptor() { return sockfd; }
    int GetFamily() { return family; }

    SocketAddress Getpeername();
    SocketAddress Getsockname();

    void Listen(int backlog);

    int Send(const void* buf, size_t len, int flags, std::error_code& ec);
    void Send(const void* buf, size_t len);
    void Send(const std::string& buf);

    void SendAll(const void* buf, size_t len);
    void SendAll(const std::string& buf);

    int Recv(void* buf, size_t len, int flags, std::error_code& ec);
    int Recv(void* buf, size_t len);
    std::string Recv(size_t len);

    void RecvAll(void* buf, size_t len);
    std::string RecvAll(size_t len);

    template <typename OptValT>
    void Setsockopt(int level, int optname, const OptValT& optval);

    void Shutdown(int how);

    int SendTo(const void* buf, size_t len, int flags, const SocketAddress& dest_addr, std::error_code& ec);
    void SendTo(const void* buf, size_t len, const SocketAddress& dest_addr);
    void SendTo(const std::string& buf, const SocketAddress& dest_addr); 

    int RecvFrom(void* buf, size_t len, int flags, SocketAddress& src_addr, std::error_code& ec);
    int RecvFrom(void* buf, size_t len, SocketAddress& src_addr);
    std::tuple<std::string, SocketAddress> RecvFrom(size_t len);

private:
    int sockfd = -1;
    int family = -1;
};

template <typename OptValT>
void Socket::Setsockopt(int level, int optname, const OptValT& optval) {
    if (setsockopt(sockfd, level, optname, &optval, sizeof(OptValT)) < 0)
        ThrowSystemError("Setsockopt(%d, %d) error", level, optname);
}

