# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(wrapsock-cxx LANGUAGES CXX)

file(GLOB wrapsock-cxx-srcs ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

# generate an object library from sources
add_library(wrapsock-cxx-objs
  OBJECT
    ${wrapsock-cxx-srcs}
  )

# this is only needed for older compilers
# but doesn't hurt either to have it
set_target_properties(wrapsock-cxx-objs
  PROPERTIES
    POSITION_INDEPENDENT_CODE 1
  )

add_library(wrapsock-cxx-shared
  SHARED
    $<TARGET_OBJECTS:wrapsock-cxx-objs>
  )

set_target_properties(wrapsock-cxx-shared
    PROPERTIES
    OUTPUT_NAME "wrapsock-cxx"
    )

add_library(wrapsock-cxx-static
  STATIC
    $<TARGET_OBJECTS:wrapsock-cxx-objs>
  )

set_target_properties(wrapsock-cxx-static
    PROPERTIES
    OUTPUT_NAME "wrapsock-cxx"
    )

