#pragma once

#include <stdarg.h>
#include <stdexcept>

static void error_handling(const char *format, ...)
{
    const int buf_size = 1024;
    char    buf[buf_size] = {'\0'};
    va_list ap;

    va_start (ap, format);
    vsnprintf(buf, buf_size, format, ap);
    va_end (ap);
    throw std::runtime_error(buf);
}

