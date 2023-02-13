#include <string>
#include <sstream>
#include <iostream>
#include <gflags/gflags.h>
#include "wrapsock.hpp"

void server(const char *host, uint16_t port) {
    Socket sock(AF_INET, SOCK_STREAM);
    sock.Setsockopt(SOL_SOCKET, SO_REUSEADDR, 1);
    sock.Bind(SocketAddress(host, port));
    sock.Listen(1);
    std::cout << "Listening at " << sock.Getsockname().ToString() << std::endl;
#if __cplusplus >= 201703L
    auto [sc, sockname] = sock.Accept(); // only for c++17
#else
    Socket sc;
    SocketAddress sockname;
    std::tie(sc, sockname) = sock.Accept();
#endif
    std::cout << "Accepted connection from " << sockname << std::endl;
    sc.Shutdown(SHUT_WR);
    std::string message;
    char buf[8192];
    while (true) {
        auto n = sc.Recv(buf, sizeof(buf));
        if (n == 0) {
            std::cout << "Received zero bytes - end of file" << std::endl;
            break;
        }
        std::cout << "Received " << n << " bytes" << std::endl;
        message.append(buf, n);
    }
    std::cout << "Message:\n" << std::endl;
    std::cout << message << std::endl;
    sc.Close();
    sock.Close();
}

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "Send and receive over TCP\n\n"
        << "Usage: " << prog << " [--help] [--host HOST] [--port PORT]";
    return os.str();
}

DEFINE_string(host, "0.0.0.0", "IP address the server listens at");
DEFINE_int32(port, 1060, "TCP port number");

int main(int argc, char *argv[]) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    server(FLAGS_host.c_str(), FLAGS_port);

    return 0;
}
