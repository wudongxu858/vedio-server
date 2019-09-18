#ifndef QSQLITE_H
#define QSQLITE_H
#include"sqlite3.h"
#include "../ctool.h"
#include <stdio.h>
#include <string.h>
#include <functional>
#include <pthread.h>
class MySqlite
{
public:
#define SQL_ARG char **Result,int Row,int Col
    typedef std::function<void(SQL_ARG)> SQLFUN;
    static MySqlite* getsqlite(void);

    void getdata(SQLFUN fun,const char *sql,...);
    void getdata(const char *sql,...);
    ~MySqlite();
private:
    MySqlite();
    pthread_mutex_t mutex;//线程访问安全
    sqlite3 *db;    
    static bool isDefine;
    static MySqlite *mysql;
};

#endif // QSQLITE_H
