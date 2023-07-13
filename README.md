## linux_timer

### 介绍

- 该仓库代码适用于Linux平台下的定时器模块
- 使用`SIGEV_THREAD`方式创建定时器, 通过线程回调函数实现定时器功能, 不会占用主程序执行时间

### 使用说明

- 具体使用方式参考[示例代码](https://github.com/hu-submodule-demo/linux_timer_demo)
- 编译时需要添加`-lrt`选项
