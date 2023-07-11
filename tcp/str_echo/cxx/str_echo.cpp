#include "str_echo.hpp"
#include "unp.hpp"

void
str_echo(Socket& sock)
{
    const int buf_size = 1024;
    while (true) {
        std::string buf = sock.Recv(buf_size);
        if (buf.empty()) {
            return;
        }
        sock.SendAll(buf);
    }
}
