#include <asio.hpp>
#include <iostream>
#include <string>
#include "error_handling.hpp"

static std::string read_line(asio::ip::tcp::socket& sock)
{
    static asio::streambuf buf;
    static std::istream input(&buf);

    asio::read_until(sock, buf, '\n');
    std::string response;
    std::getline(input, response);

    return response;
}

static void echo_client(std::istream& in, asio::ip::tcp::socket& sock)
{
    std::string sendline, recvline;

	while (std::getline(std::cin, sendline)) {
        sendline += '\n';

        asio::write(sock, asio::buffer(sendline));

        recvline = read_line(sock);
		if (recvline.empty())
			error_handling("str_cli: server terminated prematurely");

        std::cout << recvline << std::endl;
	}
}

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

    asio::io_context ioc;

    // Step 3. Creating and opening a socket.
    asio::ip::tcp::socket sock(ioc, ep.protocol());

    // Step 4. Connecting a socket.
    sock.connect(ep);

    // Step 5. Send and receive message with socket.
    echo_client(std::cin, sock);

    return 0;
}
