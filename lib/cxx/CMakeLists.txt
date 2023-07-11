# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(unp-lib LANGUAGES CXX)

# generate a library from sources
add_library(unp 
    STATIC 
    blocks.cpp
    blocks.hpp
    error.hpp
    socket_address.cpp
    socket_address.hpp
    unp.hpp
    wraplib.cpp
    wraplib.hpp
    wrapnetdb.cpp
    wrapnetdb.hpp
    wrapsock.cpp
    wrapsock.hpp
    wrapunix.cpp
    wrapunix.hpp
    format.hpp
    read_buffer.cpp
    read_buffer.hpp
    sock_ntop.cpp
    sock_ntop.hpp
    )
