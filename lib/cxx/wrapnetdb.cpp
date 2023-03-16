#include "wrapnetdb.hpp"
#include "error.hpp"

#include <cassert>
#include <arpa/inet.h>

class hostent_category_t : public std::error_category {
public:
    const char* name() const noexcept override { return "hostent"; }
    std::error_condition default_error_condition (int ev) const noexcept override {
        switch (ev) {
        case 0:
            return std::error_condition(hostent_errc::success);
        case HOST_NOT_FOUND:
            return std::error_condition(hostent_errc::unknown_host);
        case NO_DATA:
            return std::error_condition(hostent_errc::no_address_associated_with_name);
        case NO_RECOVERY:
            return std::error_condition(hostent_errc::unknown_server_error);
        case TRY_AGAIN:
            return std::error_condition(hostent_errc::host_name_lookup_failure);
        default:
            return std::error_condition(hostent_errc::other);
        }
    }

    bool equivalent(const std::error_code& code, int condition) const noexcept override {
        return *this == code.category() &&
            static_cast<int>(default_error_condition(code.value()).value())==condition;
    }

    std::string message(int ev) const override {
        return hstrerror(ev);
    }
};

const std::error_category& hostent_category() noexcept {
    static hostent_category_t category;
    return category;
}

std::error_condition make_error_condition (hostent_errc e) {
    return std::error_condition(static_cast<int>(e), hostent_category());
}

std::vector<std::string> Gethostbyname(const std::string& name, std::error_code& ec) {
    std::vector<std::string> addr_list;
    struct hostent *hptr;
    if ((hptr = gethostbyname(name.c_str())) == NULL) {
        ec.assign(h_errno, hostent_category());
        return addr_list;
    }
    int addrtype = hptr->h_addrtype;
    assert(addrtype == AF_INET);
    char buf[INET_ADDRSTRLEN];
    for (char **pptr = hptr->h_addr_list; *pptr != NULL; pptr++) {
        if (NULL == inet_ntop(addrtype, *pptr, buf, sizeof(buf))) {
            PrintSystemError("inet_ntop() error");
            continue;
        }
        addr_list.push_back(buf);
    }
    return addr_list;
}

std::vector<std::string> Gethostbyname(const std::string& name) {
    std::error_code ec;
    auto addr_list = Gethostbyname(name, ec);
    if (ec) {
        ThrowSystemErrorWithCode(ec, "Gethostbyname(%s) error", name.c_str());
    }
    return addr_list;
}
