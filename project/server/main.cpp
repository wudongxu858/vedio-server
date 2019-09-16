
#include "Server.h"
#include "sqlite3.h"
void signal_fun(int num)
{
    printf("get signal:%d\n",num);
}
int main()
{
    signal(SIGPIPE,signal_fun);//管道破裂信号
    key_t cem_key = ftok("/tmp", 1);
    key_t sharemem_key=ftok("/tmp",2);
    key_t memback_key=ftok("/tmp",3);
    pid_t pid=fork();
    if(pid<0)
    {
        perror("FORK");
        return 0;
    }
    else if(pid>0)//father
    {      
        PreServer preser(cem_key,sharemem_key,memback_key);
        preser.Run();
    }
    else //son
    {
        PostServer posser(cem_key,sharemem_key,memback_key);
        posser.Run();
    }
    return 0;
}





