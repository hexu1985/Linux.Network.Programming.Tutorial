pybind11:
https://github.com/pybind/pybind11.git
https://gitee.com/luskliu/pybind11.git


### 构建netlink模块

```
$ bash build_by_cmake.sh
```

### 打包uevent_monitor.py

```
$ bash dist_by_pyinstaller.sh
```

### signal处理
在python中以线程方式调用C++模块的阻塞api, 会造成线程的C++模块把SIGINT吞掉的情况,
参见uevent_monitor2.py, 目前想到的处理方式是在netlink.cpp里增加signal处理, 
直接调用exit, 参加netlink_signal.cpp
后来发现是因为通过pybind11调用的C++模块的阻塞api, 把整个python进程都给锁死了
