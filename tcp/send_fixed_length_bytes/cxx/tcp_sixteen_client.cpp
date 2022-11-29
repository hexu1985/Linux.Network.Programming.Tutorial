#include <string>
#include <sstream>
#include <iostream>
#include <gflags/gflags.h>
#include "wrapsock.hpp"

void client(const char* host, uint16_t port) {
    Socket sock(AF_INET, SOCK_STREAM);
    sock.Connect(host, port);
    std::cout << "Client has been assigned socket name " << sock.Getsockname() << std::endl;
    sock.SendAll("Hi there, server");
    auto reply = sock.RecvAll(16);
    std::cout << "The server said " << reply << std::endl;
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
