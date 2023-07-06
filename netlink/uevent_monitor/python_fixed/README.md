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
通过pybind11调用的C++模块的阻塞api, 把整个python进程都给锁死了,
这里解决方法采用在C++模块中启动native thread, 通过回调的方式通知python
