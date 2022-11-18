基于C++标准库实现定时器Timer类
==============================

定时器timer是多线程编程中经常设计到的工具类
简单的定时器原理其实很简单（是不是有点GNU is not unix的味道;）：

- 创建一个新线程
- 在那个线程里等待
- 等待指定时长后做任务

python标准库中就有这么一个定时器类：threading.Timer
此类表示一个操作应该在等待一定的时间之后运行 --- 相当于一个定时器。 
Timer 类是 Thread 类的子类，因此可以像一个自定义线程一样工作。
与线程一样，通过调用 start() 方法启动定时器。 而 cancel() 方法可以停止计时器（在计时结束前）。

例如：

```python
def hello():
    print("hello, world")

t = Timer(30.0, hello)
t.start()  # after 30 seconds, "hello, world" will be printed
```

**class threading.Timer(interval, function, args=None, kwargs=None)**
    创建一个定时器，在经过 interval 秒的间隔事件后，将会用参数 args 和关键字参数 kwargs 调用 function。
    如果 args 为 None （默认值），则会使用一个空列表。如果 kwargs 为 None （默认值），则会使用一个空字典。

cancel()
    停止定时器并取消执行计时器将要执行的操作。仅当计时器仍处于等待状态时有效。

接下来我将给出两种C++的Timer实现，接口类似于python的threading.Timer，不过精度为秒，原因有二：

- 实现代码参考了Posix多线程编程里的alarm程序，为了方便大家对比基于pthread的C语言实现，这里也以秒为单位
- 避免一上来过多的涉及C++标准库中chrono中对于时间转化的函数的使用，使代码逻辑更清晰的集中在timer相关的部分

当然，作为一个在产品级代码中可用的Timer，精度至少应该在毫米才行，所以文章最后也会给出精度在微秒的完整实现。

首先，给出C++版本的Timer的接口定义：
几乎完全仿照python的threading.Timer,

```cpp
class Timer {
public:
    typedef std::function<void ()> Callback;

    Timer(int interval, Callback function); 
    void Start();
    void Cancel(); 
};
```

- Callback：类型为std::function<void ()>，即返回类型为void的“函数”，当然在C++里可以是普通函数，函数对象，lambda等。
- Timer(interval, function)：构造函数，创建一个Timer，interval秒后到期（相对于调用Start函数时的时间点），回调函数为function。
- Start()：启动定时器
- Cancel()：停止定时器并取消执行计时器将要执行的操作。

在给出C++的实现前，我们先给出测试驱动程序。测试驱动程序是从《Posix多线程程序设计》（英文原版书名为Programming with POSIX Threads）里的实例代码而来：闹钟程序。
我接下来的介绍顺序如下：
- 先给出书中基于pthread的多线程版本的实例代码，C代码
- 将C版本的代码转化成等价的python代码，基于threading.Timer接口实现的版本
- 将python版本的代码，转化成基于Timer类接口的版本，形成Timer的测试驱动代码
- 实现C++版的Timer类，并且编译测试驱动代码，运行验证

那么，我先给出基于pthread的多线程版本的实例代码，C代码：

```c
/*
 * alarm_fork.c
 *
 * This version of alarm.c uses pthread_create to create a
 * separate thread to wait for each alarm to expire.
 */
#include <pthread.h>
#include "errors.h"

typedef struct alarm_tag {
    int         seconds;
    char        message[64];
} alarm_t;

void *alarm_thread (void *arg)
{
    alarm_t *alarm = (alarm_t*)arg;
    int status;

    status = pthread_detach (pthread_self ());
    if (status != 0)
        err_abort (status, "Detach thread");
    sleep (alarm->seconds);
    printf ("(%d) %s\n", alarm->seconds, alarm->message);
    free (alarm);
    return NULL;
}
int main (int argc, char *argv[])
{
    int status;
    char line[128];
    alarm_t *alarm;
    pthread_t thread;

    while (1) {
        printf ("Alarm> ");
        if (fgets (line, sizeof (line), stdin) == NULL) exit (0);
        if (strlen (line) <= 1) continue;
        alarm = (alarm_t*)malloc (sizeof (alarm_t));
        if (alarm == NULL)
            errno_abort ("Allocate alarm");

        /*
         * Parse input line into seconds (%d) and a message
         * (%64[^\n]), consisting of up to 64 characters
         * separated from the seconds by whitespace.
         */
        if (sscanf (line, "%d %64[^\n]", 
            &alarm->seconds, alarm->message) < 2) {
            fprintf (stderr, "Bad command\n");
            free (alarm);
        } else {
            status = pthread_create (
                &thread, NULL, alarm_thread, alarm);
            if (status != 0)
                err_abort (status, "Create alarm thread");
        }
    }
}
```
代码的完整说明参见《Posix多线程程序设计》 1.5.3章节，这里就不再搬运原文了。

接下来是移殖的python版本代码：
```python
#!/usr/bin/env python3

from threading import Timer

class Alarm:
    def __init__(self, seconds:int, message:str):
        self.seconds = seconds
        self.message = message

def callback(alarm:Alarm):
    print("({}) {}\n".format(alarm.seconds, alarm.message))

if __name__ == "__main__":
    while True:
        line = input("Alarm> ")
        if len(line) <= 1:
            continue

        try:
            seconds, *message = line.split(' ')
            alarm = Alarm(int(seconds), ' '.join(message))
            t = Timer(interval=int(seconds), function=callback, args=(alarm, ))
            t.start()
        except:
            print("Bad command")

```
python版本的代码大家有兴趣的可以在本地运行一下，看看效果;)

再然后，我们把这段代码翻译成C++版本的
```cpp
#include "timer.hpp"
#include <cstdlib>
#include <string>
#include <memory>
#include <iostream>

struct Alarm {
    Alarm(int seconds_, const std::string& message_): 
        seconds(seconds_), message(message_) {
    }

    int seconds;
    std::string message;
};

void callback(std::shared_ptr<Alarm> alarm) {
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
            Timer t(seconds, std::bind(callback, alarm));
            t.Start();
        } 
        catch (const std::exception& e) {
            std::cout << "Bad command" << std::endl;
        }
    }
}
```
这样我们就有了C++版本Timer的测试驱动程序，并且可以跟C和python版本的代码运行对比。

接下来给出C++版本的Timer实现：
```cpp
#pragma once

#include <thread>
#include <chrono>
#include <atomic>
#include <functional>

class Timer {
public:
    typedef std::function<void ()> Callback;

    Timer(int interval, Callback function) {
        pimpl = std::make_shared<Impl>(interval, function);
    }

    void Start() {
        std::thread t([pimpl=pimpl]() {
            if(!pimpl->active.load()) return;
            std::this_thread::sleep_for(std::chrono::seconds(pimpl->interval));
            if(!pimpl->active.load()) return;
            pimpl->function();
        });
        t.detach();
    }

    void Cancel() {
        pimpl->active.store(false);
    }

private:
    struct Impl {
        Impl(int interval_, Callback function_): interval(interval_), function(function_) {}

        int interval;
        Callback function;
        std::atomic<bool> active{true};
    };

private:
    std::shared_ptr<Impl> pimpl;
};
```
基本上是C版本的代码抽离和封装，并把相关函数替换成C++标准库的实现而已。不过Timer类麻雀虽小，但五脏俱全，用的了C++标准库中的：
- std::function：用于抽象到期回调函数
- std::shared_ptr：用于管理Timer::Impl的生命周期
- std::atomic：用于Cancel Timer，保证线程安全
- std::thread：用于Timer线程，sleep指定时间，然后调用回调函数
- std::chrono：C++标准库的时间类都在其中实现
- C++ lambda：Timer线程的target函数，捕获了this->pimpl，保证了Timerl::Impl对象不会因为Timer对象的析构而析构

还用的了Pimpl惯用法，这里把接口和实现都放在了头文件里，标准的做法是Timer的成员函数实现和Timer::Impl实现已经除了std::shared_ptr和std::function以外的依赖，
都可以挪到.cpp文件里。

这个Timer类的实现优缺点是十分的明显，优点是代码简洁，一目了然，缺点是太过简洁，每个Timer需要一个线程去运行，系统资源消耗大。
于是就引出了基于条件变量版本的Timer，实现“参考”了《Posix多线程程序设计》3.3.4章节提到闹钟实例的最终版本（与其说“参考”，改称“直译”也不为过;）。

照例，我先给出基于pthread的条件变量版本的实例代码，C代码：
```c
/*
 * alarm_cond.c
 *
 * This is an enhancement to the alarm_mutex.c program, which
 * used only a mutex to synchronize access to the shared alarm
 * list. This version adds a condition variable. The alarm
 * thread waits on this condition variable, with a timeout that
 * corresponds to the earliest timer request. If the main thread
 * enters an earlier timeout, it signals the condition variable
 * so that the alarm thread will wake up and process the earlier
 * timeout first, requeueing the later request.
 */
#include <pthread.h>
#include <time.h>
#include "errors.h"

/*
 * The "alarm" structure now contains the time_t (time since the
 * Epoch, in seconds) for each alarm, so that they can be
 * sorted. Storing the requested number of seconds would not be
 * enough, since the "alarm thread" cannot tell how long it has
 * been on the list.
 */
typedef struct alarm_tag {
    struct alarm_tag    *link;
    int                 seconds;
    time_t              time;   /* seconds from EPOCH */
    char                message[64];
} alarm_t;

pthread_mutex_t alarm_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t alarm_cond = PTHREAD_COND_INITIALIZER;
alarm_t *alarm_list = NULL;
time_t current_alarm = 0;

/*
 * Insert alarm entry on list, in order.
 */
void alarm_insert (alarm_t *alarm)
{
    int status;
    alarm_t **last, *next;

    /*
     * LOCKING PROTOCOL:
     * 
     * This routine requires that the caller have locked the
     * alarm_mutex!
     */
    last = &alarm_list;
    next = *last;
    while (next != NULL) {
        if (next->time >= alarm->time) {
            alarm->link = next;
            *last = alarm;
            break;
        }
        last = &next->link;
        next = next->link;
    }
    /*
     * If we reached the end of the list, insert the new alarm
     * there.  ("next" is NULL, and "last" points to the link
     * field of the last item, or to the list header.)
     */
    if (next == NULL) {
        *last = alarm;
        alarm->link = NULL;
    }
#ifdef DEBUG
    printf ("[list: ");
    for (next = alarm_list; next != NULL; next = next->link)
        printf ("%d(%d)[\"%s\"] ", next->time,
            next->time - time (NULL), next->message);
    printf ("]\n");
#endif
    /*
     * Wake the alarm thread if it is not busy (that is, if
     * current_alarm is 0, signifying that it's waiting for
     * work), or if the new alarm comes before the one on
     * which the alarm thread is waiting.
     */
    if (current_alarm == 0 || alarm->time < current_alarm) {
        current_alarm = alarm->time;
        status = pthread_cond_signal (&alarm_cond);
        if (status != 0)
            err_abort (status, "Signal cond");
    }
}

/*
 * The alarm thread's start routine.
 */
void *alarm_thread (void *arg)
{
    alarm_t *alarm;
    struct timespec cond_time;
    time_t now;
    int status, expired;

    /*
     * Loop forever, processing commands. The alarm thread will
     * be disintegrated when the process exits. Lock the mutex
     * at the start -- it will be unlocked during condition
     * waits, so the main thread can insert alarms.
     */
    status = pthread_mutex_lock (&alarm_mutex);
    if (status != 0)
        err_abort (status, "Lock mutex");
    while (1) {
        /*
         * If the alarm list is empty, wait until an alarm is
         * added. Setting current_alarm to 0 informs the insert
         * routine that the thread is not busy.
         */
        current_alarm = 0;
        while (alarm_list == NULL) {
            status = pthread_cond_wait (&alarm_cond, &alarm_mutex);
            if (status != 0)
                err_abort (status, "Wait on cond");
            }
        alarm = alarm_list;
        alarm_list = alarm->link;
        now = time (NULL);
        expired = 0;
        if (alarm->time > now) {
#ifdef DEBUG
            printf ("[waiting: %d(%d)\"%s\"]\n", alarm->time,
                alarm->time - time (NULL), alarm->message);
#endif
            cond_time.tv_sec = alarm->time;
            cond_time.tv_nsec = 0;
            current_alarm = alarm->time;
            while (current_alarm == alarm->time) {
                status = pthread_cond_timedwait (
                    &alarm_cond, &alarm_mutex, &cond_time);
                if (status == ETIMEDOUT) {
                    expired = 1;
                    break;
                }
                if (status != 0)
                    err_abort (status, "Cond timedwait");
            }
            if (!expired)
                alarm_insert (alarm);
        } else
            expired = 1;
        if (expired) {
            printf ("(%d) %s\n", alarm->seconds, alarm->message);
            free (alarm);
        }
    }
}

int main (int argc, char *argv[])
{
    int status;
    char line[128];
    alarm_t *alarm;
    pthread_t thread;

    status = pthread_create (
        &thread, NULL, alarm_thread, NULL);
    if (status != 0)
        err_abort (status, "Create alarm thread");
    while (1) {
        printf ("Alarm> ");
        if (fgets (line, sizeof (line), stdin) == NULL) exit (0);
        if (strlen (line) <= 1) continue;
        alarm = (alarm_t*)malloc (sizeof (alarm_t));
        if (alarm == NULL)
            errno_abort ("Allocate alarm");

        /*
         * Parse input line into seconds (%d) and a message
         * (%64[^\n]), consisting of up to 64 characters
         * separated from the seconds by whitespace.
         */
        if (sscanf (line, "%d %64[^\n]", 
            &alarm->seconds, alarm->message) < 2) {
            fprintf (stderr, "Bad command\n");
            free (alarm);
        } else {
            status = pthread_mutex_lock (&alarm_mutex);
            if (status != 0)
                err_abort (status, "Lock mutex");
            alarm->time = time (NULL) + alarm->seconds;
            /*
             * Insert the new alarm into the list of alarms,
             * sorted by expiration time.
             */
            alarm_insert (alarm);
            status = pthread_mutex_unlock (&alarm_mutex);
            if (status != 0)
                err_abort (status, "Unlock mutex");
        }
    }
}
```

代码有些长，代码说明见《Posix多线程程序设计》3.3.4章节，我这里啰嗦几句，简单总结一下，基于条件变量实现定时器的原理：
- 程序维护了一个有序的定时器列表，顺序按照到期时间从小到大排列
- 独立的alarm_thread线程函数，实现定时器到期回调的逻辑，通过条件变量的pthread_cond_wait函数，处理定时器列表为空的情况，以及等待队列中最早的定时器到期，以及有更早的定时器插入队列的情况
- 函数alarm_insert函数，实现将定时器插入到定时器列表的逻辑，并在定时器列表原先为空的情况下，或者在插入的定时器到期时间早于定时间列表中最早到期时，通知alarm_thread线程，唤醒pthread_cond_wait函数。

由于基于pthread的条件变量版本的实例代码中，增加了DEBUG宏和调试代码，所以我的C++版本的测试驱动程序也需要做相应的调整：
```cpp
#include "timer.hpp"
#include <cstdlib>
#include <string>
#include <memory>
#include <iostream>

struct Alarm {
    Alarm(int seconds_, const std::string& message_): 
        seconds(seconds_), message(message_) {
    }

    int seconds;
    std::string message;
};

void callback(std::shared_ptr<Alarm> alarm) {
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
        if (!std::getline(std::cin, line)) quick_exit(0);
        if (line.length() <= 1) continue;

        try {
            std::tie(seconds, message) = parse_command(line);
            auto alarm = std::make_shared<Alarm>(seconds, message);
            Timer t(seconds, std::bind(callback, alarm));
#ifdef DEBUG
            t.SetMessage(message);
#endif
            t.Start();
        } 
        catch (const std::exception& e) {
            std::cout << "Bad command" << std::endl;
        }
    }
}
```
其实修改就只有两处：
![alarm_cond](png/alarm_cond.png)

接下来给出C++的实现，如果C版本的代码看得懂，那C++版本的代码就可以说是一目了然，比较C++的版本是“直译”过来的。
```cpp
#pragma once

#include <functional>
#include <memory>

class Timer {
public:
    typedef std::function<void ()> Callback;

    Timer(int interval, Callback function); 
    void Start();
    void Cancel(); 

#ifdef DEBUG
    void SetMessage(const std::string& message);
#endif

public:
    struct Impl; 

private:
    std::shared_ptr<Impl> pimpl;
};
```
首先来看Timer的头文件，这里就看出Pimpl惯用法的优势了，头文件里完全剥离了对<chrono>、<thread>、<atomic>的依赖。
另外，增加了SetMessage接口，用于实现C版本中DEBUG宏中的调试信息打印

下面是Timer的源文件，代码和C版本的一样，有点儿长：
```cpp
#include "timer.hpp"
#include <list>
#include <memory>
#include <chrono>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>

#ifdef DEBUG
#include <iostream>
#endif

using Clock = std::chrono::system_clock;
using TimePoint = Clock::time_point; 
using Seconds = std::chrono::seconds;
using AlarmPtr = std::shared_ptr<Timer::Impl>;

#ifdef DEBUG
namespace {

std::ostream& operator<<(std::ostream& out, const TimePoint& tp) {
    using namespace std::chrono;
    auto d = duration_cast<seconds>(tp.time_since_epoch());
    out << d.count();
    return out;
}

template <typename Rep, typename Preiod>
std::ostream& operator<<(std::ostream& out, const std::chrono::duration<Rep, Preiod>& d) {
    using namespace std::chrono;
    auto s = duration_cast<seconds>(d);
    out << s.count();
    return out;
}

}   // namespace
#endif

class Timer::Impl {
public:
    Impl(int interval_, Callback function_): interval(interval_), function(function_) {
    }

    void CalculateTime() {
        time = Clock::now() + Seconds(interval);
    }

    int interval;
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

    void ThreadSafetyInsert(AlarmPtr alarm) {
        std::unique_lock<std::mutex> lock(alarm_mutex);
        Insert(alarm);
    }

    void Insert(AlarmPtr alarm);
    void Run();

private:
    std::list<AlarmPtr> alarm_list;
    TimePoint current_alarm;
    std::mutex alarm_mutex;
    std::condition_variable alarm_cond;
};

/*
 * Insert alarm entry on list, in order.
 */
void AlarmLooper::Insert(AlarmPtr alarm) {
    auto first = alarm_list.begin();
    auto last = alarm_list.end();

    /*
     * LOCKING PROTOCOL:
     * 
     * This routine requires that the caller have locked the
     * alarm_mutex!
     */
    for ( ; first != last; ++first) {
        if ((*first)->time >= alarm->time) {
            alarm_list.insert(first, alarm);
            break;
        }
    }
    /*
     * If we reached the end of the list, insert the new alarm
     * there.  ("next" is NULL, and "last" points to the link
     * field of the last item, or to the list header.)
     */
    if (first == last) {
        alarm_list.push_back(alarm);
    }
#ifdef DEBUG
    std::cout << "[list:";
    for (auto item : alarm_list) {
        std::cout << item->time << "(" << (item->time - Clock::now()) << ")[\""
                << item->message << "\"] ";
    }
    std::cout << "]\n" << std::flush;
#endif
    /*
     * Wake the alarm thread if it is not busy (that is, if
     * current_alarm is 0, signifying that it's waiting for
     * work), or if the new alarm comes before the one on
     * which the alarm thread is waiting.
     */
    if (current_alarm == TimePoint{} || alarm->time < current_alarm) {
        current_alarm = alarm->time;
        alarm_cond.notify_one();
    }
}

/*
 * The alarm thread's start routine.
 */
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
            std::cout << "[waiting: " << alarm->time << "(" << (alarm->time - Clock::now()) << ")\""
                << alarm->message << "\"\n" << std::flush; 
#endif
            current_alarm = alarm->time;
            while (current_alarm == alarm->time) {
                status = alarm_cond.wait_until(lock, alarm->time);
                if (status == std::cv_status::timeout) {
                    expired = true;
                    break;
                } 
            }
            if (!expired) {
                Insert(alarm);
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

class TimerThread {
public:
    TimerThread(); 
    void AddTimer(std::shared_ptr<Timer::Impl> timer);
    bool CurrentThreadIsAlarmLooperThread() {
        return std::this_thread::get_id() == looper_thread.get_id();
    }

    static TimerThread& GetInstance() {
        static TimerThread timer_thread;
        return timer_thread;
    }

private:
    AlarmLooper alarm_looper;
    std::thread looper_thread;
};

TimerThread::TimerThread() {
    looper_thread = std::thread(&AlarmLooper::Run, &alarm_looper);
    looper_thread.detach();
}

void TimerThread::AddTimer(std::shared_ptr<Timer::Impl> timer) {
    if (CurrentThreadIsAlarmLooperThread()) {
        alarm_looper.Insert(timer);
    } else {
        alarm_looper.ThreadSafetyInsert(timer);
    }
}

Timer::Timer(int interval, Callback function) {
    pimpl = std::make_shared<Impl>(interval, function);
}

void Timer::Start() {
    pimpl->CalculateTime();
    TimerThread::GetInstance().AddTimer(pimpl);
}

void Timer::Cancel() {
    pimpl->active = false;
}

#ifdef DEBUG
void Timer::SetMessage(const std::string& message) {
    pimpl->message = message;
}
#endif
```
实现原理和C版本的一致（要不说是“直译”呢;），具体的映射关系如下：
- AlarmLooper类：封装了C版本的函数alarm_thread、alarm_insert逻辑，AlarmLooper::Run对应alarm_thread，AlarmLooper::Insert对应alarm_insert
- TimerThread类：作为单例类，管理运行AlarmLooper::Run的线程
- 然后就是std::list替代了C手写的链表
- std::mutex和std::conditon_variable替代了pthread_mutex_t和pthread_cond_t结构和函数。

最后，我来“吹一吹”实现C++版本的Timer的价值，以及可以改进优化的方向。
价值方面：
- C版本的代码很优秀，用C++的面向对象方式实现，可以再C++工程中以组件的方式复用（主要是指条件变量版本，多线程版本过于简陋，不过好处是head only）
- C++版本依赖于C++标准库，而C++标准库是跨平台的，所以Timer类也是跨平台的（当然，如果把C版本的pthread函数替换成C11标准库的线程函数也能达到同样目的，但据我所知各编译器厂商对C11的多线程支持的不是很积极）
改进方面：
- 到期时间的精度是秒，后面会给出微秒级精度的实现链接（和目前的实现原理一致）
- std::list可以替换成std::multi_set以提高性能，依据是用红黑树的性能要好于链表的插入排序。
- TimerThread类管理的thread是detach的，造成在linux上，进程退出时，有可能在std::condition_variable的析构函数上把进程hang住，这个也在微秒级的实现版本中做了改进。

最后的最后，给出文章中提到的所有代码的完整实现链接：
- 秒级的实现，包括C版本的原始代码，python版本的实例代码，C++的两个版本的代码，链接如下：<>
- 微秒级的实现，包括python版本的实例代码，C++的两个版本的代码，链接如下：<>


### 参考文档：

- 《Posix多线程程序设计（Programming with POSIX Threads）》
- [3.11.0 Documentation » The Python Standard Library » Concurrent Execution » threading](https://docs.python.org/3/library/threading.html)
- [A Simple Timer in C++](https://www.fluentcpp.com/2018/12/28/timer-cpp/)


