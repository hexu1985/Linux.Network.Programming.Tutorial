#include <string>
#include <iostream>

#include "error.hpp"
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

void client(const char* host, uint16_t port) {
    Socket sock(AF_INET, SOCK_STREAM);
    sock.Connect(host, port);
    std::cout << "Client has been assigned socket name ("
              << sock.Getsockname().ToString() << ")\n";
    sock.SendAll("Hi there, server");
    auto reply = recvall(sock, 16);
    std::cout << "The server said " << reply << '\n';
    sock.Close();
}

int main(int argc, char *argv[])
{
    const char* host = "127.0.0.1";
    uint16_t port = 1060;

    if (argc > 1) {
        host = argv[1];
    }
    if (argc > 2) {
        port = atoi(argv[2]);
    }

    client(host, port);

    return 0;
}
