#include "read_buffer.hpp"
#include "error.hpp"
#include <unistd.h>

ReadBuffer::ReadBuffer(int fd_, size_t buf_size): fd(fd_), read_buf(buf_size) {
    read_ptr = &read_buf[0];
}

int ReadBuffer::get_char(char *ptr)
{
	if (read_cnt <= 0) {
again:
		if ( (read_cnt = read(fd, &read_buf[0], sizeof(read_buf))) < 0) {
			if (errno == EINTR)
				goto again;
			return(-1);
		} else if (read_cnt == 0)
			return(0);
		read_ptr = &read_buf[0];
	}

	read_cnt--;
	*ptr = *read_ptr++;
	return(1);
}

std::string ReadBuffer::Readline(int maxlen, char delim) {
	ssize_t	n, rc;
	char c;
    std::string line;
	for (n = 1; n < (int) maxlen; n++) {
		if ( (rc = get_char(&c)) == 1) {
            line += c;
			if (c == delim)
				break;	/* newline is stored, like fgets() */
		} else if (rc == 0) {
            return line;
		} else
            ThrowSystemError("read() error");
	}

	return line;
}


