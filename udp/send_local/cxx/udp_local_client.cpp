#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <gflags/gflags.h>

#include "unp.hpp"

#define MAX_BYTES 65535

std::string datetime_now()
{
    char buf[255];
    memset(buf, 0, sizeof(buf));
    time_t t = time(0);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&t));
    return buf;
}

void client(uint16_t port) {
    Socket sock(AF_INET, SOCK_DGRAM);
    auto text = "The time is " + datetime_now();
    sock.SendTo(text, SocketAddress("127.0.0.1", port));
    std::cout << "The OS assigned me the address " << sock.Getsockname() << std::endl;
    SocketAddress address;
    std::tie(text, address) = sock.RecvFrom(MAX_BYTES);
    std::cout << "The server " << address << " replied " << text << std::endl;
    sock.Close();
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

    client(FLAGS_port);

    return 0;
}

