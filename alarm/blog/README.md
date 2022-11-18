基于C++标准库实现Timer类
========================

定时器timer是多线程编程中经常设计到的工具类
定时器的原理其实很简单：

- 创建一个新线程
- 在那个线程里等待
- 等待指定时长后做任务

python标准库中就有这么一个定时器类：threading.Timer
此类表示一个操作应该在等待一定的时间之后运行 --- 相当于一个定时器。 
Timer 类是 Thread 类的子类，因此可以像一个自定义线程一样工作。
与线程一样，通过调用 start() 方法启动定时器。 而 cancel() 方法可以停止计时器（在计时结束前）。

例如：

```
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

当然，作为一个在产品级代码可用的Timer，精度至少应该在毫米才算可用，所以文章最后也会给出精度在微秒的完整实现。

首先，给出C++版本的Timer的接口定义：
几乎完全仿照python的threading.Timer,

```
class Timer {
public:
    typedef std::function<void ()> Callback;

    Timer(int interval, Callback function); 
    void Start();
    void Cancel(); 
};
```

- Callback：类型为std::function<void ()>，即返回类型为void的“函数”，当然在C++里可以是普通函数，函数对象，lambda等。
- Timer(interval, function)：构造函数，创建一个Timer，interval秒后超时（相对于Timer构造时的时间点），回调函数为function。
- Start()：启动定时器
- Cancel()：停止定时器并取消执行计时器将要执行的操作。

在给出C++的实现前，我们先给出测试驱动程序。测试驱动程序是从《Posix多线程程序设计》（英文原版书名为Programming with POSIX Threads）里的示例代码而来：闹钟程序。
我接下来的介绍顺序如下：
- 先给出书中基于pthread的多线程版本的示例代码，C代码
- 将C版本的代码转化成等价的python代码，基于threading.Timer接口实现的版本
- 将python版本的代码，转化成基于Timer类接口的版本，形成Timer的测试驱动代码
- 实现C++版的Timer类，并且编译测试驱动代码，运行验证

那么，我先给出基于pthread的多线程版本的示例代码，C代码：

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
代码的完整说明参见《Posix多线程程序设计》 1.5.3章节，这里就不在搬运原文了。

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

最后，是C++版本的Timer实现：
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

