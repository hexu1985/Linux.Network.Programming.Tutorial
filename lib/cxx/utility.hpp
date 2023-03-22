#pragma once

#include <arpa/inet.h>
#include <unistd.h>
#include <sys/un.h>

#include <string>
#include <tuple>

std::string Gethostname(std::error_code& ec);
std::string Gethostname();

std::string Inet_ntop(int family, const void *addrptr, std::error_code& ec);
std::string Inet_ntop(int family, const void *addrptr);

std::string Sock_ntop(const struct sockaddr *addr, socklen_t addrlen, std::error_code& ec);
std::string Sock_ntop(const struct sockaddr *addr, socklen_t addrlen);

