/*
与MySQL交互时，应用程序应使用该一般性原则：
1.通过调用mysql_library_init()，初始化MySQL库。库可以是mysqlclient C客户端库
  ，或mysqld嵌入式服务器库，具体情况取决于应用程序是否与“-libmysqlclient”或“-libmysqld”标志链接。
2.通过调用mysql_init()初始化连接处理程序，并通过调用mysql_real_connect()连接到服务器。
3.发出SQL语句并处理其结果。（在下面的讨论中，详细介绍了使用它的方法）。
4.通过调用mysql_close()，关闭与MySQL服务器的连接。
5.通过调用mysql_library_end()，结束MySQL库的使用。
*/

#ifndef SQLCONNPOOL_H
#define SQLCONNPOOL_H

#include <mysql/mysql.h>
#include <string>
#include <queue>
#include <mutex>
#include <semaphore.h>
#include <thread>
#include "../log/log.h"

class SqlConnPool
{
public:
    
    static SqlConnPool *Instance();

    MYSQL *GetConn();

    void FreeConn(MYSQL *sql);

    int GetFreeConnCount();

    void Init(const char *host, int port,
              const char *user, const char *pwd,
              const char *dbName, int connSize);

    void ClosePool();

private:
    SqlConnPool();
    ~SqlConnPool();

    int MAX_CONN_;
    int useCount_;
    int freeCount_;

    std::queue<MYSQL *> connQue_;
    std::mutex mtx_;
    sem_t semId_;
};

#endif //SQLCONNPOOL_H