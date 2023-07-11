#include "str_cli.hpp"
#include "unp.hpp"

void
str_cli(std::istream& in, Socket& sock)
{
    std::string sendline;
    std::string recvline;
    const int buf_size = 1024;
    ReadBuffer buffer(sock.Fileno(), buf_size);
    while (std::getline(in, sendline)) {
        sendline += '\n';
        sock.SendAll(sendline);
        recvline = buffer.Readline(buf_size);
        if (recvline.empty()) {
            ThrowRuntimeError("str_cli: server terminated prematurely");
        }
        std::cout << recvline << std::flush;
    }
}
