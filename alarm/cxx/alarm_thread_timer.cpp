#include "thread_timer.hpp"
#include <string>
#include <memory>
#include <iostream>

struct Alarm {
    int seconds;
    std::string message;
};

void callback(std::shared_ptr<Alarm> alarm) {
    std::cout << "(" << alarm->seconds << ") " << alarm->message << std::endl;
}

int main()
{
}

