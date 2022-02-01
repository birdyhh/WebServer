#include "sqlconnpool.h"

SqlConnPool *SqlConnPool::Instance()
{
    static SqlConnPool connPool;
    return &connPool;
}

MYSQL* SqlConnPool::GetConn()
{

}

void SqlConnPool::FreeConn(MYSQL *conn)
{

}

int SqlConnPool::GetFreeConnCount()
{

}

void SqlConnPool::Init(const char *host, int port,
            const char *user, const char *pwd,
            const char *dbName, int connSize = 10)
{
    assert(connSize > 0);
    for (int i = 0; i < connSize;i++)
    {
        MYSQL *sql = nullptr;
        sql = mysql_init(sql);
        if(!sql)
        {
            LOG_ERROR("MySql init error!");
            assert(sql);
        }
        sql = mysql_real_connect(sql, host,
                                 user, pwd,
                                 dbName, port, nullptr, 0);
        if(!sql)
        {
            LOG_ERROR("MySql Connect error");
        }
        connQue_.push(sql);
    }
    MAX_CONN_ = connSize;
    sem_init(&semId_, 0, MAX_CONN_);
}

void SqlConnPool::ClosePool()
{

}

SqlConnPool::SqlConnPool()
{
    useCount_ = 0;
    freeCount_ = 0;
}

SqlConnPool::~SqlConnPool()
{
    ClosePool();
}