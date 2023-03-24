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
            return std::error_condition(hostent_errc::non_recoverable_failure);
        case TRY_AGAIN:
            return std::error_condition(hostent_errc::temporary_failure);
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
        ThrowSystemErrorWithCode(ec, "Gethostbyname('{}') error", name);
    }
    return addr_list;
}

class addrinfo_category_t : public std::error_category {
public:
    const char* name() const noexcept override { return "addrinfo"; }
    std::error_condition default_error_condition (int ev) const noexcept override {
        switch (ev) {
        case 0:
            return std::error_condition(addrinfo_errc::success);
        case EAI_AGAIN:
            return std::error_condition(addrinfo_errc::temporary_failure); 
        case EAI_BADFLAGS:
            return std::error_condition(addrinfo_errc::invalid_flags);
        case EAI_FAIL:
            return std::error_condition(addrinfo_errc::non_recoverable_failure);
        case EAI_FAMILY:
            return std::error_condition(addrinfo_errc::unsupported_address_family);
        case EAI_MEMORY:
            return std::error_condition(addrinfo_errc::out_of_memory);
        case EAI_NONAME:
            return std::error_condition(addrinfo_errc::unknown_node_or_service);
        case EAI_SOCKTYPE:
            return std::error_condition(addrinfo_errc::unsupported_socket_type);
        case EAI_SYSTEM:
            return std::error_condition(addrinfo_errc::system_error);
        default:
            return std::error_condition(addrinfo_errc::other);
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

const std::error_category& addrinfo_category() noexcept {
    static addrinfo_category_t category;
    return category;
}

std::error_condition make_error_condition (addrinfo_errc e) {
    return std::error_condition(static_cast<int>(e), addrinfo_category());
}

std::shared_ptr<struct addrinfo> Getaddrinfo(
        const std::string& node, const std::string& service,
        const struct addrinfo *hints, std::error_code& ec) {
    struct addrinfo* res = nullptr;
    int n = 0;
    if ((n = getaddrinfo(node.c_str(), service.c_str(), hints, &res)) != 0) {
        ec.assign(n, addrinfo_category());
        return nullptr;
    }

    auto deleter = [](struct addrinfo* res) { if (res) freeaddrinfo(res); };
    return std::shared_ptr<struct addrinfo>(res, deleter);
}

std::shared_ptr<struct addrinfo> Getaddrinfo(
        const std::string& node, const std::string& service,
        const struct addrinfo *hints) {
    std::error_code ec;
    auto res = Getaddrinfo(node, service, hints, ec);
    if (ec) {
        ThrowSystemErrorWithCode(ec, "Getaddrinfo('{}', '{}') error", node, service);
    }
    return res;
}

std::vector<SocketAddressInfo> Getaddrinfo(
        const std::string& node, const std::string& service,
        int family, int type, int protocol, int flags,
        std::error_code& ec) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = flags;
    hints.ai_family = family;
    hints.ai_socktype = type;
    hints.ai_protocol = protocol;

    auto ressave = Getaddrinfo(node, service, &hints, ec);
    if (ec) {
        return {};
    }

    std::vector<SocketAddressInfo> info_list;
    for (auto res = ressave.get(); res; res = res->ai_next) {
        SocketAddressInfo info;
        info.family = res->ai_family;
        info.type = res->ai_socktype;
        info.protocol = res->ai_protocol;
        info.canon_name = res->ai_canonname ? res->ai_canonname : "";
        info.address = SocketAddress(res->ai_addr, res->ai_addrlen);
        info_list.push_back(std::move(info));
    }
    return info_list;
}

std::vector<SocketAddressInfo> Getaddrinfo(
        const std::string& node, const std::string& service,
        int family, int type, int protocol, int flags) {
    std::error_code ec;
    auto info_list = Getaddrinfo(node, service, family, type, protocol, flags, ec);
    if (ec) {
        ThrowSystemErrorWithCode(ec, "Getaddrinfo('{}', '{}') error", node, service);
    }
    return info_list;
}

