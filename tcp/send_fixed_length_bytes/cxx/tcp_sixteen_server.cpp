#include <string>
#include <sstream>
#include <iostream>
#include <gflags/gflags.h>
#include "wrapsock.hpp"

std::string recvall(Socket& sock, int length) {
    std::string data;
    while (data.length() < length) {
        auto more = sock.Recv(length - data.length());
        if (more.empty()) {
            ThrowRuntimeError("was expecting %d bytes but only received"
                           " %d bytes before the socket closed",
                           length, data.length());
        }
        data += more;
    }
    return data;
}

void server(const char *interface, uint16_t port) {
    Socket sock(AF_INET, SOCK_STREAM);
    sock.Setsockopt(SOL_SOCKET, SO_REUSEADDR, 1);
    sock.Bind(interface, port);
    sock.Listen(1);
    std::cout << "Listening at " << sock.Getsockname().ToString() << std::endl;
    while (true) {
        std::cout << "Waiting to accept a new connection" << std::endl;
        auto [sc, sockname] = sock.Accept();
        std::cout << "We have accepted a connection from " << sockname << std::endl;
        std::cout << "  Socket name: " << sc.Getsockname() << std::endl;
        std::cout << "  Socket peer: " << sc.Getpeername() << std::endl; 
        auto message = recvall(sc, 16);
        std::cout << "  Incoming sixteen-octet message: " << message << std::endl;
        sc.SendAll("Farewell, client");
        sc.Close();
        std::cout << "  Reply sent, socket closed" << std::endl;
    }
}

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "Send and receive over TCP\n\n"
        << "Usage: " << prog << " [--help] [--host HOST] [--port PORT]";
    return os.str();
}

DEFINE_string(host, "", "interface the server listens at");
DEFINE_int32(port, 1060, "TCP port (default 1060)");

int main(int argc, char *argv[]) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    server(FLAGS_host.c_str(), FLAGS_port);

    return 0;
}
