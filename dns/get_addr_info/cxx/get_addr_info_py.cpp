#include <iostream>
#include <string>
#include <iomanip>

#include "unp.hpp"

void get_addr_info_list(const std::string& remote_host)
{
    auto result = Getaddrinfo(remote_host, "", 0, SOCK_STREAM);

    int counter = 0;
    for (const auto& item : result) {
        std::cout << std::left << std::setw(2) << counter << item.address << '\n';
        counter++;
    }
}

int main(int argc, char *argv[])
{
    std::string remote_host = "www.python.org";
    if (argc == 2) {
        remote_host = argv[1];
    }
    get_addr_info_list(remote_host);
}

