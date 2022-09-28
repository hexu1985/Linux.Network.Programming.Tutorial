#include <string>
#include <iostream>
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

int main(int argc, char *argv[]) {
    const char* host = "127.0.0.1";
    uint16_t port = 1060;

    if (argc > 1)
        host = argv[1];
    if (argc > 2)
        port = atoi(argv[2]);

    server(host, port);

    return 0;
}
