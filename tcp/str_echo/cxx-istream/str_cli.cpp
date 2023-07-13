#include "str_cli.hpp"
#include "unp.hpp"

void
str_cli(std::istream& in, Socket& sock)
{
    std::string sendline;
    std::string recvline;
    const int buf_size = 1024;
    SocketInputStream sock_stream(sock);
    ReadBuffer buffer(sock.Fileno(), buf_size);
    while (std::getline(in, sendline)) {
        sendline += '\n';
        sock.SendAll(sendline);
        std::getline(sock_stream, recvline);
        if (recvline.empty()) {
            ThrowRuntimeError("str_cli: server terminated prematurely");
        }
        recvline +='\n';
        std::cout << recvline << std::flush;
    }
}
