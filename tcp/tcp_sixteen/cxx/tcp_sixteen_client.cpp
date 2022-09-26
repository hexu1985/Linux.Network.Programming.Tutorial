#include <string>

#include "wrapsock.hpp"


void client(const std::string& host, uint16_t port) {
    Socket sock(AF_INET, SOCK_STREAM);
//    sock.Connect(host, port);
}

int main(int argc, char *argv[])
{
    std::string host = "127.0.0.1";
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
