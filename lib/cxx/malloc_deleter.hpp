#pragma once 

#include <cstdlib>

template <typename To, typename From=To>
To* malloc_helper(int n=1) {
    return reinterpret_cast<To*>(malloc(sizeof(From)*n));
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
