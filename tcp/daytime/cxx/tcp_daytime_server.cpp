#include <string>
#include <sstream>
#include <iostream>
#include <gflags/gflags.h>

#include "unp.hpp"

void server(const char *host, uint16_t port) {
    Socket sock(AF_INET, SOCK_STREAM);
    sock.Bind(SocketAddress(host, port));
    sock.Listen(5);

    char buff[128];
    time_t ticks;
    for ( ; ; ) {
#if __cplusplus >= 201703L
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
        auto [sc, _] = sock.Accept();
#pragma GCC diagnostic pop
#else
        Socket sc;
        std::tie(sc, std::ignore) = sock.Accept();
#endif
        ticks = time(0);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        
        sc.Send(buff, strlen(buff));
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
