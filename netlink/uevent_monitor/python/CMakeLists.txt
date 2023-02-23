cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(uevent_monitor LANGUAGES CXX)

add_subdirectory(pybind11)

include_directories(lib)

pybind11_add_module(uevent_monitor 
    lib/error.cpp
    lib/error.hpp
    lib/wrapsock.cpp
    lib/wrapsock.hpp
    uevent_monitor.cpp)
