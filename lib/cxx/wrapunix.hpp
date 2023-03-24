#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <errno.h>

#include <string>
#include <system_error>
#include <tuple>
#include <chrono>

#include "error.hpp"

std::string Gethostname(std::error_code& ec);
std::string Gethostname();

template <typename... Args>
int Fcntl(int fd, int cmd, Args... args) {
    int n = fcntl(fd, cmd, args...);
    if (n < 0) {
        ThrowSystemError("Fcntl({}, {}) error", fd, cmd);
    }
    return n;
}

std::tuple<std::vector<int>, std::vector<int>, std::vector<int>>
Select(const std::vector<int>& readfds, const std::vector<int>& writefds, const std::vector<int>& exceptfds, 
        struct timeval* timeout, std::error_code& ec); 

std::tuple<std::vector<int>, std::vector<int>, std::vector<int>>
Select(const std::vector<int>& readfds, const std::vector<int>& writefds, const std::vector<int>& exceptfds, 
      std::error_code& ec); 

std::tuple<std::vector<int>, std::vector<int>, std::vector<int>>
Select(const std::vector<int>& readfds, const std::vector<int>& writefds, const std::vector<int>& exceptfds, 
        struct timeval* timeout = nullptr);

template <class Rep, class Period>
std::tuple<std::vector<int>, std::vector<int>, std::vector<int>>
Select(const std::vector<int>& readfds, const std::vector<int>& writefds, const std::vector<int>& exceptfds, 
        const std::chrono::duration<Rep, Period> &timeout, std::error_code& ec) {
    double timeout_sec = std::chrono::duration<double>(timeout).count();
    struct timeval tv;
    tv.tv_sec = (long) timeout_sec;
    tv.tv_usec = (long) ((timeout_sec - tv.tv_sec)*1000000);
    return Select(readfds, writefds, exceptfds, &tv, ec);
}

template <class Rep, class Period>
std::tuple<std::vector<int>, std::vector<int>, std::vector<int>>
Select(const std::vector<int>& readfds, const std::vector<int>& writefds, const std::vector<int>& exceptfds, 
        const std::chrono::duration<Rep, Period> &timeout) {
    std::error_code ec;
    auto ret = Select(readfds, writefds, exceptfds, timeout, ec);
    if (ec) {
        ThrowSystemErrorWithCode(ec, "Select() error");
    }
    return ret;
}
