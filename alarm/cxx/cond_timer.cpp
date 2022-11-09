#include "cond_timer.hpp"
#include <chrono>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <list>

using Clock = std::chrono::system_clock;
using TimePoint = Clock::time_point; 
using Seconds = std::chrono::seconds;
using AlarmPtr = std::shared_ptr<Timer::Impl>;

class Timer::Impl {
public:
    Impl(int interval_, Callback function_): function(function_) {
        auto now = Clock::now();
        time = now + Seconds(interval_);
    }

    Timer::Callback function;
    TimePoint time;
    std::atomic<bool> active{true};

#ifdef DEBUG
    std::string message;
#endif
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
    std::list<AlarmPtr> alarm_list;
    TimePoint current_alarm;
    std::mutex alarm_mutex;
    std::condition_variable alarm_cond;
};

void AlarmLooper::Run() {
    AlarmPtr alarm;
    TimePoint now;
    bool expired;
    std::cv_status status;

    /*
     * Loop forever, processing commands. The alarm thread will
     * be disintegrated when the process exits. Lock the mutex
     * at the start -- it will be unlocked during condition
     * waits, so the main thread can insert alarms.
     */
    std::unique_lock<std::mutex> lock(alarm_mutex);
    while (true) {
        /*
         * If the alarm list is empty, wait until an alarm is
         * added. Setting current_alarm to 0 informs the insert
         * routine that the thread is not busy.
         */
        current_alarm = TimePoint{};
        while (alarm_list.empty()) {
            alarm_cond.wait(lock);
        }
        alarm = alarm_list.front();
        alarm_list.pop_front();
        now = Clock::now();
        expired = false;
        if (alarm->time > now) {
#ifdef DEBUG
            std::cout << "[waiting: " << alarm->time 
                << "(" << (alarm->time - Clock::now()) << ")"
                << '"' << alarm->message << '"'
                << std::endl;
#endif
            current_alarm = alarm->time;
            while (current_alarm == alarm->time) {
                status = alarm_cond.wait_until(lock, alarm->time);
                if (status == std::cv_status::timeout) {
                    expired = true;
                    break;
                } 
            }
        } else {
            expired = true;
        }

        if (expired) {
            if (alarm->active) {
                alarm->function();
            }
        }
    }
}
