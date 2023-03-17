#include <string>
#include <iostream>
#include "wrapnetdb.hpp"

void get_host_by_name(const std::string& remote_host) { 
    try {
        auto ip_address_list = Gethostbyname(remote_host);
        for (const auto& ip_address : ip_address_list) {
            std::cout << "IP address of " << remote_host << ": " << ip_address << std::endl;
        }
    } catch (const std::system_error& e) {
        std::cout << remote_host << ": " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::string remote_host = "www.python.org";
    if (argc == 2) {
        remote_host = argv[1];
    }
    get_host_by_name(remote_host);
    return 0;
}

