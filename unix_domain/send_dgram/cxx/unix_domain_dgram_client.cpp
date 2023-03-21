#include <unistd.h>

#include <string>
#include <sstream>
#include <iostream>
#include <gflags/gflags.h>

#include "unp.hpp"

#define MAX_BYTES 65535

std::string datetime_now()
{
    char buf[255];
    memset(buf, 0, sizeof(buf));
    time_t t = time(0);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&t));
    return buf;
}

void client(const char* path) {
    std::string mypath = std::string("/tmp/cxx_unix_socket_client.") + std::to_string((int) getpid());
    unlink(mypath.c_str());

    Socket sock(AF_UNIX, SOCK_DGRAM);
    sock.Bind(mypath.c_str());
    auto text = "The time is " + datetime_now();
    sock.SendTo(text, SocketAddress(path));
    std::cout << "The OS assigned me the address " << sock.Getsockname() << std::endl;
    SocketAddress address;
    std::tie(text, address) = sock.RecvFrom(MAX_BYTES);
    std::cout << "The server " << address << " replied " << text << std::endl;
    sock.Close();
}

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "Send and receive UNIX domain datagram locally\n\n"
        << "Usage: " << prog << " [--help] [--path PATH]";
    return os.str();
}

DEFINE_string(path, "/tmp/unix_domain_socket_server", "unix-domain address the client sends to");

int main(int argc, char *argv[]) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    client(FLAGS_path.c_str());

    return 0;
}

