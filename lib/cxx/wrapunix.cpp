#include "wrapunix.hpp"
#include "error.hpp"

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

