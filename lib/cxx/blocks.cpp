#include "blocks.hpp"

#include <cstdint>

std::string GetBlock(Socket& sock) {
    uint32_t block_length;
    sock.RecvAll(&block_length, sizeof(block_length));
    block_length = ntohl(block_length);
    return sock.RecvAll(block_length);
}

void PutBlock(Socket& sock, const std::string& message) {
    uint32_t block_length = message.length();
    block_length = htonl(block_length);
    sock.SendAll(&block_length, sizeof(block_length));
    sock.SendAll(message);
}

