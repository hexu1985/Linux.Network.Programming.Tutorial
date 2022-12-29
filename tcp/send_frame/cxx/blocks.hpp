#pragma once

#include <string>
#include "wrapsock.hpp"

std::string GetBlock(Socket& sock); 
void PutBlock(Socket& sock, const std::string& message); 
