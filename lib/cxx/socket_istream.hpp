#pragma once

#include <iostream>
#include "wrapsock.hpp"

class SocketInputStream: public std::istream {
public:
    SocketInputStream(Socket& sock);
    ~SocketInputStream();
};
