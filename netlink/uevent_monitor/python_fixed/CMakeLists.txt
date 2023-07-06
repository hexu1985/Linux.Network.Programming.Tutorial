cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(netlink LANGUAGES CXX)

add_subdirectory(pybind11)

include_directories(lib)

pybind11_add_module(netlink 
    lib/error.hpp
    lib/sock_ntop.cpp
    lib/sock_ntop.hpp
    lib/socket_address.cpp
    lib/socket_address.hpp
    lib/wraplib.cpp
    lib/wraplib.hpp
    lib/wrapsock.cpp
    lib/wrapsock.hpp
    netlink.cpp)
