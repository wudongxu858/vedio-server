#include "mysqlite.h"

MySqlite::~MySqlite()
{
    isDefine=false;
    sqlite3_close(db);
}

MySqlite::MySqlite()
{
    int ret=sqlite3_open("myData.sql",&db);
    if (ret == SQLITE_OK)
    {
        Debug("open success ");
    }
    else
    {
        Debug("open error ");
    }
}


bool MySqlite::isDefine=false;
MySqlite *MySqlite::mysql=NULL;
MySqlite * MySqlite::getsqlite(void)
{
    static MySqlite mysql;
    if(isDefine==false)
    {    
        Debug("create sqlite");
        isDefine=true;
        return &mysql;
    }
    else
    {
        return &mysql;
    }
}



void MySqlite::getdata(char *sql,SQLFUN fun)
{
    char **pResult;
    int Row=0,Col=0;
    int ret =sqlite3_get_table(db,sql,&pResult,&Row,&Col,0);
    //function(pResult,Row,Col);
    if (ret != SQLITE_OK)
    {
        Debug("get data error");
        return ;
    }
    fun(pResult,Row,Col);
}

