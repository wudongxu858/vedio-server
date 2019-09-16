#include "threadpool.h"
#include<stdio.h>
#include<string>

int main(int argc,char **argv)
{
    CThreadPll * poll =CThreadPll::GetPoll(20); 
    sleep(1); 
    bool isOut=false;
    poll->addTask([&isOut](void){
                getchar();
                isOut=true;
            });
    while(true)
    {
        for(int i=0;i<100;i++)
        {
            poll->addTask([i](void){
                Debug("pthread do =%d",i);
                //sleep(1);
            });
        }
        
        if(isOut==true)
        {
            break;
        }
    }
    poll->pool_destroy();
    return 0;
}
//lambda 表达式