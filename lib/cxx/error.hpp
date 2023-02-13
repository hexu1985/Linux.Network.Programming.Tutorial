#pragma once

#include <errno.h>
#include <string>
#include <stdexcept>
#include <system_error>
#include <iostream>

std::string error_message(const char* file, unsigned int line, const char* format, ...);

#define ThrowRuntimeError(...) \
    throw std::runtime_error(error_message(__FILE__, __LINE__, __VA_ARGS__))

#define ThrowSystemError(...) \
    throw std::system_error(errno, std::system_category(), \
            error_message(__FILE__, __LINE__, __VA_ARGS__))

#define ThrowSystemErrorWithCode(error_code, ...) \
    throw std::system_error(error_code, \
            error_message(__FILE__, __LINE__, __VA_ARGS__))

#define PrintRuntimeError(...) \
    do { \
        auto msg = error_message(__FILE__, __LINE__, __VA_ARGS__); \
        msg += '\n'; \
        std::cerr << msg; \
    } while (false)

#define PrintSystemError(...) \
    do { \
        int errno_save = errno; \
        auto msg = error_message(__FILE__, __LINE__, __VA_ARGS__); \
        msg += ": "; \
        msg += strerror(errno_save); \
        msg += '\n'; \
        std::cerr << msg; \
    } while (false)
