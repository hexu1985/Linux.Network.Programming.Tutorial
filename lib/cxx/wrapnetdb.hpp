#pragma once

#include <netdb.h>

#include <system_error>
#include <type_traits>
#include <vector>
#include <string>
#include <memory>
#include <tuple>

#include "socket_address.hpp"

enum class hostent_errc {
    success = 0,
    unknown_host = HOST_NOT_FOUND,
    no_address_associated_with_name = NO_DATA,
    non_recoverable_failure = NO_RECOVERY,
    temporary_failure = TRY_AGAIN,
    other = 255
};

namespace std {
template<> struct is_error_condition_enum<hostent_errc> : public true_type {};
}   // namespace std

const std::error_category& hostent_category() noexcept;
std::error_condition make_error_condition(hostent_errc e); 

std::vector<std::string> Gethostbyname(const std::string& name);
std::vector<std::string> Gethostbyname(const std::string& name, std::error_code& ec);

enum class addrinfo_errc {
    success = 0,
    temporary_failure = EAI_AGAIN,
    invalid_flags = EAI_BADFLAGS,
    non_recoverable_failure = EAI_FAIL,
    unsupported_address_family = EAI_FAMILY,
    out_of_memory = EAI_MEMORY,
    unknown_node_or_service = EAI_NONAME,
    unsupported_socket_type = EAI_SOCKTYPE,
    system_error = EAI_SYSTEM,
    other = 255
};

namespace std {
template<> struct is_error_condition_enum<addrinfo_errc> : public true_type {};
}   // namespace std

const std::error_category& addrinfo_category() noexcept;
std::error_condition make_error_condition(addrinfo_errc e); 

std::shared_ptr<struct addrinfo> Getaddrinfo(
        const std::string& node, const std::string& service,
        const struct addrinfo *hints, std::error_code& ec);

std::shared_ptr<struct addrinfo> Getaddrinfo(
        const std::string& node, const std::string& service,
        const struct addrinfo *hints);

struct SocketAddressInfo {
    int family;
    int type;
    int protocol;
    std::string canon_name;
    SocketAddress address;
};

std::vector<SocketAddressInfo> Getaddrinfo(
        const std::string& node, const std::string& service="",
        int family=0, int type=0, int protocol=0, int flags=0);


