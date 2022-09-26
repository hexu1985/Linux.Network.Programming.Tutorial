#pragma once

#include <errno.h>
#include <string>
#include <stdexcept>
#include <system_error>

std::string error_message(const char* file, unsigned int line, const char* format, ...);

#define ThrowRuntimeError(format, ...) \
    throw std::runtime_error(error_message(__FILE__, __LINE__, format, __VA_ARGS__))

#define ThrowSystemError(format, ...) \
    throw std::system_error(errno, std::system_category(), \
            error_message(__FILE__, __LINE__, format, __VA_ARGS__))

