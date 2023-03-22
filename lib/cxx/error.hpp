#pragma once

#include <sstream>
#include "format.hpp"

template <typename... Args>
inline
std::string format_error_message(const char* file, unsigned int line, 
        const std::string& format, Args&&... args) {
    std::ostringstream os;
    os << "[" << file << ":" << line << "] " << Format(format, args...);
    return os.str();
}

#define ThrowRuntimeError(...) \
    throw std::runtime_error(format_error_message(__FILE__, __LINE__, __VA_ARGS__))

#define ThrowSystemError(...) \
    throw std::system_error(errno, std::system_category(), \
            format_error_message(__FILE__, __LINE__, __VA_ARGS__))

#define ThrowSystemErrorWithCode(error_code, ...) \
    throw std::system_error(error_code, \
            format_error_message(__FILE__, __LINE__, __VA_ARGS__))

#define PrintRuntimeError(...) \
    do { \
        auto msg = format_error_message(__FILE__, __LINE__, __VA_ARGS__); \
        msg += '\n'; \
        std::cerr << msg; \
    } while (false)

#define PrintSystemError(...) \
    do { \
        int errno_save = errno; \
        auto msg = format_error_message(__FILE__, __LINE__, __VA_ARGS__); \
        msg += ": "; \
        msg += strerror(errno_save); \
        msg += '\n'; \
        std::cerr << msg; \
    } while (false)
