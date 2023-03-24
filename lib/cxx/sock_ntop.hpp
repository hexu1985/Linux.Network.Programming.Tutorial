#pragma once

#include <arpa/inet.h>
#include <sys/un.h>

#include <string>
#include <system_error>

std::string Sock_ntop(const struct sockaddr *addr, socklen_t addrlen, std::error_code& ec);
std::string Sock_ntop(const struct sockaddr *addr, socklen_t addrlen);

