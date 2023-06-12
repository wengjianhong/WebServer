## 基于C++11的Web服务端项目

### 项目介绍

本项目基于C++11编写的Web服务端项目，目前

- 仅实现了Get请求
- 支持HTTP/1.0、HTTP/1.1
- 支持长连接请求



### 开发环境

- 系统：CentOS Linux release 7.9.2009 (Core)
- 编译器：g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-44)
- 编译工具：cmake3 version 3.17.5



### 使用说明

```bash
# 下载/克隆代码
git clone git@github.com:wengjianhong/WebServer.git
# 创建编译目录
mkdir build && cd build
# 编译源码
cmake3 .. && make
# 查看使用说明
./test_webserver --help
# 运行示例
./test_webserver --port 1080 --path ../web
```



### 技术说明

- 使用Reactor模式
- 使用Epoll边沿触发的IO多路复用技术
- 使用多线程充分利用多核CPU，并使用线程池避免线程频繁创建销毁的开销



### 设计模型

并发模型为Reactor+非阻塞IO+线程池，新连接Round Robin分配，详细介绍请参考[《Reactor和Proactor》](https://github.com/wengjianhong/Notes/blob/master/01.%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80/%E9%AB%98%E6%80%A7%E8%83%BD/Reactor%E5%92%8CProactor%E6%A8%A1%E5%9E%8B.md) 中的单Reactor多线程实现



### 代码统计

```bash
[root@QingYun WebServer]# cloc .
      16 text files.
      16 unique files.
       4 files ignored.

github.com/AlDanial/cloc v 1.70  T=0.03 s (470.3 files/s, 41991.4 lines/s)
-------------------------------------------------------------------------------
Language                     files          blank        comment           code
-------------------------------------------------------------------------------
C/C++ Header                     8             97            114            468
C++                              3             86             29            425
CMake                            1              3              0             14
HTML                             2              4              0             10
-------------------------------------------------------------------------------
SUM:                            14            190            143            917
-------------------------------------------------------------------------------
```





参考：

- git@github.com:linyacool/WebServer.git