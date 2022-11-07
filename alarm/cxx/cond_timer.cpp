#include "cond_timer.hpp"

class Timer::Impl {
public:
    Impl(int interval_, Callback function_): function(function_) {
        auto now = Timer::TimePoint::now();
        time = now + std::chrono::seconds(interval_);
    }

    void Stop() {
        active.store(false);
    }

private:
    Timer::Callback function;
    Timer::TimePoint time;
    std::atomic<bool> active{true};
};

class AlarmLooper {

private:
    std::mutex alarm_mutex;
    std::mutex alarm_cond;
};
