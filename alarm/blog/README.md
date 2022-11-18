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
