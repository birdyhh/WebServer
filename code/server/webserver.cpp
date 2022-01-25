#include "webserver.h"

using namespace std;

WebServer::WebServer(
        int port, int triMode, int timeoutMS, bool OptLinger,
        int sqlPort, const char *sqlUser, const char *sqlPwd,
        const char *dbName, int connPoolnum, int threadNum,
        bool openLog, int logLevel, int logQueSize):
        port_(port),openLinger_(OptLinger),timeoutMS_(timeoutMS),isClose_(false),
        timer_(new HeapTimer()),threadpool_(new ThreadPool(threadNum)),
        epoller_(new Epoller())
        {

        }
)