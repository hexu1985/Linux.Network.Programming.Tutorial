#pragma once

#include <string>
#include <vector>

class ReadBuffer {
public:
    ReadBuffer(int fd, size_t buf_size);
    std::string Readline(int maxlen, char delim='\n');

private:
    int get_char(char* ptr);

private:
    int fd = -1;
    bool eof = false;
    std::vector<char> read_buf;
    int read_cnt = 0;
    char *read_ptr = nullptr;
};
