#include "thread_timer.hpp"
#include <ctime>
#include <cstdlib>
#include <string>
#include <memory>
#include <iostream>
#include <chrono>

struct Alarm {
    Alarm(int seconds_, const std::string& message_): 
        seconds(seconds_), message(message_) {
    }

    int seconds;
    std::string message;
};

std::string strftime(const char* format, const std::chrono::time_point<std::chrono::system_clock>& tp) {
    time_t rawtime = std::chrono::system_clock::to_time_t(tp);
    char mbstr[100];
    std::strftime(mbstr, sizeof(mbstr), format, localtime(&rawtime));
    return std::string(mbstr);
}

void callback(std::shared_ptr<Alarm> alarm) {
    std::cout << "alarm timer at " 
        << strftime("%Y-%m-%d %H:%M:%S", std::chrono::system_clock::now()) << '\n';
    std::cout << "(" << alarm->seconds << ") " << alarm->message << std::endl;
}

std::tuple<int, std::string> parse_command(const std::string& line) {
    auto pos = line.find(' ');
    if (pos == std::string::npos)
        throw std::runtime_error("invalid line: separator not found");

    int seconds = std::stoi(line.substr(0, pos));
    std::string message = line.substr(pos+1);
    return std::make_tuple(seconds, message);
}

int main()
{
    std::string line;
    int seconds;
    std::string message;
    while (true) {
        std::cout << "Alarm> ";
        if (!std::getline(std::cin, line)) exit(0);
        if (line.length() <= 1) continue;

        try {
            std::tie(seconds, message) = parse_command(line);
            auto alarm = std::make_shared<Alarm>(seconds, message);
            std::cout << "start timer at " 
                << strftime("%Y-%m-%d %H:%M:%S", std::chrono::system_clock::now()) << '\n';
            Timer t(seconds, std::bind(callback, alarm));
            t.Start();
        } 
        catch (const std::exception& e) {
            std::cout << "Bad command" << std::endl;
        }
    }
}

