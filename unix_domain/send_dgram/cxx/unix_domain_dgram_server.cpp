#include <unistd.h>
#include <string>
#include <sstream>
#include <iostream>
#include <gflags/gflags.h>
#include "wrapsock.hpp"

#define MAX_BYTES 65535

void server(const char* path) {
    Socket sock(AF_UNIX, SOCK_DGRAM);
    unlink(path);
    sock.Bind(SocketAddress(path));
    std::cout << "Listening at " << sock.Getsockname().ToString() << std::endl;
    while (true) {
#if __cplusplus >= 201703L
        auto [text, address] = sock.RecvFrom(MAX_BYTES);
#else
        std::string text;
        SocketAddress address;
        std::tie(text, address) = sock.RecvFrom(MAX_BYTES);
#endif
        std::cout << "The client at " << address << " says " << text << std::endl;
        text = "Your data was " + std::to_string(text.size()) + " bytes long";
        sock.SendTo(text, address);
    }
}

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "Send and receive UNIX domain datagram locally\n\n"
        << "Usage: " << prog << " [--help] [--path PATH]";
    return os.str();
}

DEFINE_string(path, "/tmp/unix_domain_socket_server", "unix-domain address the server listens at");

int main(int argc, char *argv[]) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    server(FLAGS_path.c_str());

    return 0;
}

