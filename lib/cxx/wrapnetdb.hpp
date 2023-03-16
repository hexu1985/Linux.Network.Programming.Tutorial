#pragma once

#include <netdb.h>

#include <system_error>
#include <type_traits>
#include <vector>
#include <string>

enum class hostent_errc {
    success = 0,
    unknown_host = HOST_NOT_FOUND,
    no_address_associated_with_name = NO_DATA,
    unknown_server_error = NO_RECOVERY,
    host_name_lookup_failure = TRY_AGAIN,
    other = 255
};

namespace std {
template<> struct is_error_condition_enum<hostent_errc> : public true_type {};
}   // namespace std

const std::error_category& hostent_category() noexcept;
std::error_condition make_error_condition (hostent_errc e); 

std::vector<std::string> Gethostbyname(const std::string& name);
std::vector<std::string> Gethostbyname(const std::string& name, std::error_code& ec);
