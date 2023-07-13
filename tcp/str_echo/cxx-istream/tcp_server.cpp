#include <string>
#include <sstream>
#include <iostream>
#include <gflags/gflags.h>

#include "unp.hpp"
#include "str_echo.hpp"

void server(const char *host, uint16_t port) {
    Socket sock(AF_INET, SOCK_STREAM);
    sock.Setsockopt(SOL_SOCKET, SO_REUSEADDR, 1);
    sock.Bind(SocketAddress(host, port));
    sock.Listen(1);
    std::cout << "Listening at " << sock.Getsockname().ToString() << std::endl;
    while (true) {
        std::cout << "Waiting to accept a new connection" << std::endl;
#if __cplusplus >= 201703L
        auto [sc, sockname] = sock.Accept();
#else
        Socket sc;
        SocketAddress sockname;
        std::tie(sc, sockname) = sock.Accept();
#endif
        std::cout << "We have accepted a connection from " << sockname << std::endl;
        std::cout << "  Socket name: " << sc.Getsockname() << std::endl;
        std::cout << "  Socket peer: " << sc.Getpeername() << std::endl; 

        str_echo(sc);
        sc.Close();
    }
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
