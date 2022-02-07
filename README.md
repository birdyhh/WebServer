# WebServer
C++ Linux WebServer
用c++14实现高性能的WEB服务器，经过webbanch压力测试可以实现上万的QPS

## 流程
```

初始化 => 主线程Epoll_wait()接收事件 => 事件多路分发 => 调用事件注册的接口 => 多线程处理事件 => 循环回到Epoll_wait()继续接收事件

```

## 功能
* 利用IO复用技术Epoll与线程池实现多线程的Reactor高并发模型；
* 允许事件驱动的应用通过多路分发的机制去处理来自不同客户端的多个请求
* 基于小根堆实现的定时器，关闭超时的非活动连接；
* 利用单例模式与阻塞队列实现日志系统，记录服务器的运行状态；
* 利用RAII机制管理内存，防止内存溢出。

## 环境要求
* Linux
* C++14
* MySql
```bash
需要配置好对应的数据库
// 建立数据库
create database yourdb;

//创建user表
USE yourdb;
CREATE TABLE user{
    username char(50) NULL,
    password char(50) NULL
}ENGINE=InnoDB;

//添加数据
INSERT INTO user(username,password) VALUES('name','password');
```

## 项目启动
```bash
make
./bin/server
```

