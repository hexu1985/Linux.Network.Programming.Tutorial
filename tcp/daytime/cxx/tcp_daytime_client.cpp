#include <string>
#include <sstream>
#include <iostream>
#include <gflags/gflags.h>

#include "unp.hpp"

void client(const char* host, uint16_t port) {
    Socket sock(AF_INET, SOCK_STREAM);
    sock.Connect(SocketAddress(host, port));
    std::string recvline;

    while (true) {
        auto recvline = sock.Recv(128);
        if (recvline.empty())
            break;

        std::cout << recvline;
    }

    sock.Close();
}

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "Send and receive over TCP\n\n"
        << "Usage: " << prog << " [--help] [--host HOST] [--port PORT]";
    return os.str();
}

DEFINE_string(host, "127.0.0.1", "IP address the client sends to");
DEFINE_int32(port, 1060, "TCP port number");

int main(int argc, char *argv[]) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    client(FLAGS_host.c_str(), FLAGS_port);

    return 0;
}
