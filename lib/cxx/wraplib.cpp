#include "wraplib.hpp"
#include "error.hpp"

std::string Inet_ntop(int family, const void *addrptr, std::error_code& ec) {
    char buf[INET6_ADDRSTRLEN] = {0};
    const char* ptr;
    if ((ptr = inet_ntop(family, addrptr, buf, sizeof(buf))) == NULL) {
        ec.assign(errno, std::system_category());
        return "";
    }
    return ptr;
}

std::string Inet_ntop(int family, const void *addrptr) {
    std::error_code ec;
    auto addr_str = Inet_ntop(family, addrptr, ec);
    if (ec) {
        ThrowSystemErrorWithCode(ec, "Inet_ntop() error");
    }
    return addr_str;
}

