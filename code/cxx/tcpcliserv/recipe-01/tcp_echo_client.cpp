#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include "error_handling.hpp"

using namespace boost;

int main(int argc, char *argv[])
{
	if (argc != 3)
        error_handling("usage: %s <IPaddress> <Port>", argv[0]);

	// Step 1. Assume that the client application has already
	// obtained the IP address and protocol port number of the
	// target server.
	std::string raw_ip_address = argv[1];
	unsigned short port_num = std::stoi(argv[2]);

    // Step 2. Creating an endpoint designating 
    // a target server application.
    asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);

    asio::io_service ios;

    // Step 3. Creating and opening a socket.
    asio::ip::tcp::socket sock(ios, ep.protocol());

    // Step 4. Connecting a socket.
    sock.connect(ep);

	return 0;
}
