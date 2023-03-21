#include <iostream>
#include <string>
#include <iomanip>

#include "unp.hpp"

void get_addr_info_list(const std::string& remote_host)
{
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;

    auto ressave = Getaddrinfo(remote_host, "", &hints);
    res = ressave.get();
    int counter = 0;
    do {
        std::cout << std::left << std::setw(2) << counter 
            << ": " << Sock_ntop(res->ai_addr, res->ai_addrlen) << '\n';
        counter++;
    } while ( (res = res->ai_next) != NULL);
}

int main(int argc, char *argv[])
{
    std::string remote_host = "www.python.org";
    if (argc == 2) {
        remote_host = argv[1];
    }
    get_addr_info_list(remote_host);
}

