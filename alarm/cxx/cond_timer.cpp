#include "cond_timer.hpp"
#include <chrono>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <forward_list>

using Clock = std::chrono::system_clock;
using TimePoint = Clock::time_point; 
using Seconds = std::chrono::seconds;

class Timer::Impl {
public:
    Impl(int interval_, Callback function_): function(function_) {
        auto now = Clock::now();
        time = now + Seconds(interval_);
    }

    void Stop() {
        active.store(false);
    }

private:
    Timer::Callback function;
    TimePoint time;
    std::atomic<bool> active{true};
};

class AlarmLooper {
public:
    AlarmLooper() = default; 
    AlarmLooper(const AlarmLooper&) = delete;
    void operator=(const AlarmLooper&) = delete;

    void InsertAlarmThreadSafety();
    void InsertAlarm();
    void Run();

private:
    using AlarmPtr = std::shared_ptr<Timer::Impl>;
    std::forward_list<AlarmPtr> alarm_list;
    TimePoint current_alarm;
    std::mutex alarm_mutex;
    std::condition_variable alarm_cond;
};

