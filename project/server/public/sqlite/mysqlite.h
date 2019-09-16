#ifndef QSQLITE_H
#define QSQLITE_H
#include"sqlite3.h"
#include "../ctool.h"
#include <stdio.h>
#include <string.h>
#include <functional>
class MySqlite
{
public:
#define SQL_ARG char **Result,int Row,int Col
    typedef std::function<void(SQL_ARG)> SQLFUN;
    static MySqlite* getsqlite(void);

    void getdata(char *sql,SQLFUN fun);
    ~MySqlite();
private:
    MySqlite();

    sqlite3 *db;    
    static bool isDefine;
    static MySqlite *mysql;
};

#endif // QSQLITE_H
