#include "error.hpp"
#include "utility.hpp"

#include <string>

std::string Gethostname(std::error_code& ec) {
    char host_name[128] = {0};
    if (gethostname(host_name, sizeof(host_name)) < 0) {
        ec.assign(errno, std::system_category());
        return "";
    }
    return host_name;
}

std::string Gethostname() {
    std::error_code ec;
    auto host_name = Gethostname(ec);
    if (ec) {
        ThrowSystemErrorWithCode(ec, "Gethostname() error");
    }
    return host_name;
}

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

std::string Sock_ntop(const struct sockaddr *addr, socklen_t addrlen, std::error_code& ec) {
    switch (addr->sa_family) {
    case AF_INET: {
        struct sockaddr_in *sin = (struct sockaddr_in *) addr;

        auto addr_str = Inet_ntop(AF_INET, &sin->sin_addr, ec);
        if (addr_str.empty()) {
            return "";
        }
        if (ntohs(sin->sin_port) != 0) {
            return addr_str+":"+std::to_string(ntohs(sin->sin_port));
        }
        return addr_str;
    }

    case AF_INET6: {
        struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *) addr;

        auto addr_str = Inet_ntop(AF_INET6, &sin6->sin6_addr, ec);
        if (addr_str.empty()) {
            return "";
        }
        if (ntohs(sin6->sin6_port) != 0) {
            return "["+addr_str+"]:"+std::to_string(ntohs(sin6->sin6_port));
        }
        return addr_str;
    }

	case AF_UNIX: {
		struct sockaddr_un *unp = (struct sockaddr_un *) addr;

			/* OK to have no pathname bound to the socket: happens on
			   every connect() unless client calls bind() first. */
		if (unp->sun_path[0] == 0)
            return "(no pathname bound)";
		else
            return unp->sun_path;
	}

    default:
        PrintRuntimeError("sock_ntop: unknown AF_xxx: %d, len %d", addr->sa_family, addrlen);
        ec.assign(EAFNOSUPPORT, std::system_category());
        return "";
    }
    return "";
}

std::string Sock_ntop(const struct sockaddr *addr, socklen_t addrlen) {
    std::error_code ec;
    auto addr_str = Sock_ntop(addr, addrlen, ec);
    if (ec) {
        ThrowSystemErrorWithCode(ec, "Sock_ntop() error");
    }
    return addr_str;
}

std::tuple<std::string, int> Sock_inX_ntop(const struct sockaddr *addr, socklen_t addrlen, std::error_code& ec) {
    switch (addr->sa_family) {
    case AF_INET: {
        struct sockaddr_in *sin = (struct sockaddr_in *) addr;

        auto addr_str = Inet_ntop(AF_INET, &sin->sin_addr, ec);
        if (addr_str.empty()) {
            return {};
        }
        return std::make_tuple(addr_str, ntohs(sin->sin_port));
    }

    case AF_INET6: {
        struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *) addr;

        auto addr_str = Inet_ntop(AF_INET6, &sin6->sin6_addr, ec);
        if (addr_str.empty()) {
            return {};
        }
        return std::make_tuple(addr_str, ntohs(sin6->sin6_port));
    }

    default:
        PrintRuntimeError("sock_ntop: unknown AF_xxx: %d, len %d", addr->sa_family, addrlen);
        ec.assign(EAFNOSUPPORT, std::system_category());
        return {};
    }
    return {};
}

std::tuple<std::string, int> Sock_inX_ntop(const struct sockaddr *addr, socklen_t addrlen) {
    std::error_code ec;
    auto addr_tuple = Sock_inX_ntop(addr, addrlen, ec);
    if (ec) {
        ThrowSystemErrorWithCode(ec, "Sock_inX_ntop() error");
    }
    return addr_tuple;
}
