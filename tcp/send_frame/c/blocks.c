#include "blocks.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static size_t buf_size = 0;
static char *read_buf = NULL;

const char *GetBlock(int sockfd)
{
    uint32_t block_length;
    RecvAll(sockfd, &block_length, sizeof(block_length));
    block_length = ntohl(block_length);
    if (buf_size < block_length+1) {
        buf_size = block_length+1;
        read_buf = realloc(read_buf, buf_size);
    }
    RecvAll(sockfd, read_buf, block_length);
    read_buf[block_length] = '\0';
    return read_buf;
}

void PutBlock(int sockfd, const char *message) {
    uint32_t block_length = strlen(message);
    block_length = htonl(block_length);
    Send(sockfd, &block_length, sizeof(block_length), 0);
    Send(sockfd, message, strlen(message), 0);
}

