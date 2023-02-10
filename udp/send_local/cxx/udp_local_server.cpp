#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <gflags/gflags.h>
#include "wrapsock.hpp"

#define MAX_BYTES 65535

void server(uint16_t port) {
    Socket sock(AF_INET, SOCK_DGRAM);
    sock.Bind("127.0.0.1", port);
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
    os << "Send and receive UDP locally\n\n"
        << "Usage: " << prog << " [--help] [--port PORT]";
    return os.str();
}

DEFINE_int32(port, 1060, "TCP port number");

int main(int argc, char *argv[]) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    server(FLAGS_port);

    return 0;
}

