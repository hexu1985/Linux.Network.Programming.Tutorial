#include "sock_ntop.hpp"
#include "error.hpp"
#include "wraplib.hpp"

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
        PrintRuntimeError("sock_ntop: unknown AF_xxx: {}, len {}", addr->sa_family, addrlen);
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

