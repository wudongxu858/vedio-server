#ifndef _THREADPOOL_H
#define _THREADPOOL_H
#include <iostream>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>
#include<list>
#include<functional>
#include<stdio.h>
#include<string>
#include "../ctool.h"
using namespace std;

class CThreadPll
{
public:
    typedef std::function<void(void)>FUNCTION;
private:   
    explicit CThreadPll(int maxCount);
    ~CThreadPll(){}
    typedef struct DataInfo_t
    {
        bool is_temp_thread;
        FUNCTION temp_function;
    }DataInfo;
public:
    static void *pthread_run(void *pdata);
    void pool_destroy();
    static CThreadPll* GetPoll(int maxCount=20);
    void wake_up(int index); 
    void addTask(FUNCTION && pTask);
private:
    
    void init_thread();
    list<FUNCTION >Task_queue;
    list<pthread_t >Thread_id_list;
    int temp_count_;
    int max_temp_count_;
    int cur_count_;
    int wait_count_;
    int max_count;
    bool is_shut_down;
    pthread_cond_t cond_t;//条件变量
    pthread_mutex_t queue_lock;
};

#endif