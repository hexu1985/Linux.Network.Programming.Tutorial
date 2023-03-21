cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(netlink LANGUAGES CXX)

add_subdirectory(pybind11)

include_directories(lib)

pybind11_add_module(netlink 
    lib/error.cpp
    lib/error.hpp
    lib/utility.cpp
    lib/utility.hpp
    lib/socket_address.cpp
    lib/socket_address.hpp
    lib/wrapsock.cpp
    lib/wrapsock.hpp
    netlink.cpp)
