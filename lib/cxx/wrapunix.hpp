#pragma once

#include <unistd.h>
#include <string>
#include <system_error>

std::string Gethostname(std::error_code& ec);
std::string Gethostname();

