#pragma once

#include <arpa/inet.h>

#include <string>
#include <system_error>

std::string Inet_ntop(int family, const void *addrptr, std::error_code& ec);
std::string Inet_ntop(int family, const void *addrptr);

