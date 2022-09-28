#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <string>
#include <system_error>

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

    std::string ToString() const;

    struct sockaddr* GetAddrPtr() { return addr; }
    const struct sockaddr* GetAddrPtr() const { return addr; }

    socklen_t* GetAddrLenPtr() { return &addrlen; }
    const socklen_t* GetAddrLenPtr() const { return &addrlen; }

private:
    sockaddr* addr = nullptr;
    socklen_t addrlen = 0;
};

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

    void Connect(const char* host, uint16_t port);
    void Connect(const SocketAddress &sock_addr);

    void Bind(const char* host, uint16_t port);
    void Bind(const SocketAddress &sock_addr);

    void Listen(int backlog);

    std::tuple<Socket, SocketAddress> Accept();

    SocketAddress Getsockname();

    void SendAll(const std::string& data);
    void SendAll(const void *ptr, size_t nbytes);

    int Recv(void *ptr, size_t nbytes, int flags, std::error_code& ec);
    int Recv(void *ptr, size_t nbytes, int flags=0);
    std::string Recv(size_t nbytes, int flags=0);

    void Close();

    int GetDescriptor() { return sockfd; }
    int GetFamily() { return family; }

private:
    int sockfd = -1;
    int family;
};
