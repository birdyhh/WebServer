#ifndef __WEBSERVER_H_
#define __WEBSERVER_H_

#include <unordered_map>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "epoller.h"
#include "../log/log.h"
#include "../timer/heaptimer.h"
#include "../pool/sqlconnpool.h"
#include "../pool/sqlconnRAII.h"
#include "../pool/threadpool.h"
#include "../http/httpconn.h"

class WebServer{
public:
    WebServer(
        int port, int triMode, int timeoutMS, bool OptLinger,/* 端口模式 ET模式 timeoutms 优雅退出*/
        int sqlPort, const char *sqlUser, const char *sqlPwd,/*Mysql配置*/
        const char *dbName, int connPoolnum, int threadNum,/*连接池数量 线程池数量 */
        bool openLog, int logLevel, int logQueSize);/*日志开关 日志等级 日志异步队列容量*/
    ~WebServer();
    void Start();

private:
    //初始化Socket
    bool InitSocket_();
    //初始化epoll事件模式
    void InitEventMode_(int trigMode);
    //添加客户端
    void AddClient_(int fd, sockaddr_in addr);

    //处理监听事件
    void DealListen_();
    //处理写事件
    void DealWrite_(HttpConn* client);
    //处理读事件
    void DealRead_(HttpConn* client);

    //发送错误
    void SendError_(int fd, const char *info);
    //
    void ExtentTime_(HttpConn *client);
    //关闭连接
    void CloseConn_(HttpConn *client);

    void OnRead_(HttpConn* client);
    void OnWrite_(HttpConn* client);
    void OnProcess_(HttpConn* client);

    static const int MAX_FD = 65536;

    //将文件描述符设置为非阻塞
    static int SetFdNonblock(int fd);

    int port_; //端口
    bool openLinger_;
    int timeoutMS_;//毫秒
    bool isClose_;
    int listenFd_;
    char *srcDir_;

    uint32_t listenEvent_;
    uint32_t connEvent_;

    std::unique_ptr<HeapTimer> timer_;
    std::unique_ptr<ThreadPool> threadpool_;
    std::unique_ptr<Epoller> epoller_;
    std::unordered_map<int, HttpConn> users_;
};

#endif