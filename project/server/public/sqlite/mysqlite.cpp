#include "mysqlite.h"

MySqlite::~MySqlite()
{
    isDefine=false;
    sqlite3_close(db);
}

MySqlite::MySqlite()
{
    pthread_mutex_init(&mutex,NULL);
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



void MySqlite::getdata(SQLFUN fun,const char *sql,...)
{
    char ch[2048];
    va_list ap;
    va_start(ap, sql);
    sprintf(ch, sql, ap);
    va_end(ap);
    
    char **pResult;
    int Row=0,Col=0;

    pthread_mutex_lock(&mutex);
    int ret =sqlite3_get_table(db,ch,&pResult,&Row,&Col,0);
    pthread_mutex_unlock(&mutex);
    //function(pResult,Row,Col);
    if (ret != SQLITE_OK)
    {
        Debug("get data error");
        return ;
    }
    fun(pResult,Row,Col);
}
void MySqlite::getdata(const char *sql,...)
{
    char ch[2048];
    va_list ap;
    va_start(ap, sql);
    sprintf(ch, sql, ap);
    va_end(ap);
    
    char **pResult;
    int Row=0,Col=0;
    pthread_mutex_lock(&mutex);
    int ret =sqlite3_get_table(db,ch,&pResult,&Row,&Col,0);
    pthread_mutex_unlock(&mutex);
    if (ret != SQLITE_OK)
    {
        Debug("get data error");
        return ;
    }
}
