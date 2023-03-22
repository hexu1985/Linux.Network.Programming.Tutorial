# set minimum cmake version
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name and language
project(unp-lib LANGUAGES C)

# generate a library from sources
add_library(unp 
    STATIC 
    blocks.c
    blocks.h
    connect_nonb.c
    connect_nonb.h
    error.c
    error.h
    unp.h
    utility.c
    utility.h
    wrapnetdb.c
    wrapnetdb.h
    wrapsock.c
    wrapsock.h
    wrapunix.c
    wrapunix.h
    )
