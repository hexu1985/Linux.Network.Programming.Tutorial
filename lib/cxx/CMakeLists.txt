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
    utility.cpp
    utility.hpp
    wrapnetdb.cpp
    wrapnetdb.hpp
    wrapsock.cpp
    wrapsock.hpp
    wrapunix.cpp
    wrapunix.hpp
    )
