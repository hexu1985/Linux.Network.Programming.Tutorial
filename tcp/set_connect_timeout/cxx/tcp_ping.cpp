#include <string>
#include <sstream>
#include <iostream>
#include <gflags/gflags.h>

#include "unp.hpp"

void connect_to(const std::string& host, const std::string& serv) {
    std::vector<SocketAddressInfo> infolist;
    try 
    {
        infolist = Getaddrinfo(
                host, serv, 0, SOCK_STREAM, 0,
                AI_ADDRCONFIG | AI_V4MAPPED | AI_CANONNAME
                );
    }
    catch (const std::system_error& e)
    {
        std::cout << "Name service failure: " << e.what() << std::endl;
        exit(1);
    }

    auto& info = infolist[0];
    Socket s(info.family, info.type, info.protocol);
    try 
    {
        s.Connect(info.address, 5);
        std::cout << "Success: host " << info.canon_name
                << " is listening on port " << info.address.IPPort()
                << std::endl;
    }
    catch (const std::system_error& e)
    {
        std::cout << "Network failure: " << e.what() << std::endl;
    }
}

std::string usage(const char* prog) {
    std::ostringstream os;
    os << "Try connecting by tcp\n\n"
        << "Usage: " << prog << " [--help] [--host HOST] [--serv SERV]";
    return os.str();
}

DEFINE_string(host, "", "hostname that you want to contact");
DEFINE_string(serv, "www", "TCP service name or port number");

int main(int argc, char *argv[]) {
    gflags::SetUsageMessage(usage(argv[0]));
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    if (FLAGS_host.empty()) {
        std::cout << usage(argv[0]) << std::endl;
        exit(1);
    }

    connect_to(FLAGS_host, FLAGS_serv);

    return 0;
}
