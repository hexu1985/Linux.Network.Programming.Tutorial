#pragma once 

#include <cstdlib>

inline
char* malloc_wrapper(size_t size) {
    return static_cast<char*>(malloc(size));
}

template <typename T>
class malloc_deleter {
public:
    void operator()(T* ptr) const {
        free(ptr);
    }
};

template <typename T>
class malloc_deleter<T[]> {
public:
    void operator()(T* ptr) const {
        free(ptr);
    }
};
