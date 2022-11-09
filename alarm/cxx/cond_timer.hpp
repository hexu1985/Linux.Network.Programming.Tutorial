#pragma once

#include <chrono>
#include <functional>
#include <memory>

class Timer {
public:
    typedef std::function<void ()> Callback;

    Timer(int interval, Callback function); 
    void start();
    void stop(); 

public:
    struct Impl; 

private:
    std::shared_ptr<Impl> pimpl;
};


