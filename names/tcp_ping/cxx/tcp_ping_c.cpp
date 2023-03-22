#include <string>
#include <sstream>
#include <iostream>
#include <gflags/gflags.h>

#include "unp.hpp"

void connect_to(const std::string& host, const std::string& serv) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_ADDRCONFIG | AI_V4MAPPED | AI_CANONNAME;

    std::error_code ec;
    auto res = Getaddrinfo(host, serv, &hints, ec);
    if (ec) {
        std::cout << "Name service failure: " << ec.message() << std::endl;
        exit(1);
    }

    Socket sock(res->ai_family, res->ai_socktype, res->ai_protocol);
    SocketAddress address(res->ai_addr, res->ai_addrlen);
    sock.Connect(address, ec);
    if (!ec) {
        std::cout << "Success: host " << (res->ai_canonname ? res->ai_canonname : "")
            << " is listening on port " << address.IPPort() << std::endl;
    } else {
        std::cout << "Network failure: " << ec.message() << std::endl;
        exit(1);
    }
}

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "Try connecting by tcp\n\n"
        << "Usage: " << prog << " [--help] [--host HOST] [--serv SERV]";
    return os.str();
}

DEFINE_string(host, "", "hostname that you want to contact");
DEFINE_string(serv, "www", "TCP service name or port number");

int main(int argc, char *argv[]) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    if (FLAGS_host.empty()) {
        std::cout << usage(argv[0]) << std::endl;
        exit(1);
    }

    connect_to(FLAGS_host, FLAGS_serv);

    return 0;
}
