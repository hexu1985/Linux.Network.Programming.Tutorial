#include <string>
#include <iostream>
#include "wrapsock.hpp"
#include "wrapnetdb.hpp"

void print_machine_info() { 
    auto host_name = Gethostname();
    std::cout << "Host name: " << host_name << std::endl;

    auto ip_address_list = Gethostbyname(host_name);
    for (const auto& ip_address : ip_address_list) {
        std::cout << "IP address: " << ip_address << std::endl;
    }
}

int main() {
    print_machine_info();
    return 0;
}
