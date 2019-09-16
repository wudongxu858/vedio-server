#include"threadpool.h"
void CThreadPll::pool_destroy()
{
    pthread_mutex_lock(&(CThreadPll::GetPoll()->queue_lock));
    if(this->is_shut_down==true)
        return ;
    this->is_shut_down=true;
    pthread_mutex_unlock(&(CThreadPll::GetPoll()->queue_lock));
    pthread_cond_broadcast(&CThreadPll::GetPoll()->cond_t);
    auto ir= CThreadPll::GetPoll()->Thread_id_list.begin();
    auto ed= CThreadPll::GetPoll()->Thread_id_list.end();
    
    for(;ir!=ed;++ir)
    {
        pthread_join(*ir,NULL);
    }
    pthread_mutex_destroy(&CThreadPll::GetPoll()->queue_lock);
    pthread_cond_destroy(&CThreadPll::GetPoll()->cond_t);
    this->temp_count_=0;
    this->wait_count_=0;
    this->cur_count_=0;
    this->max_count=0;
    this->max_temp_count_=0;
    this->is_shut_down=false;
    printf("destroy over\n");
    
}
CThreadPll::CThreadPll(int maxCount)
{
    this->temp_count_=0;
    this->wait_count_=0;
    this->cur_count_=0;
    this->max_count=maxCount;
    this->max_temp_count_=50;
    this->is_shut_down=false;
    init_thread();
    sleep(1);
}
void CThreadPll::init_thread()
{
    if(pthread_cond_init(&(this->cond_t),NULL)<0)
    {
        perror("cond init");
    }
    if(pthread_mutex_init(&(this->queue_lock),NULL)<0)
    {
        perror("mutex init");
    }
    for(int i=0;i<this->max_count;i++)
    {    
        pthread_t id;    
        DataInfo_t *data= new DataInfo_t;
        data->is_temp_thread=false;
        pthread_create(&id,NULL,CThreadPll::pthread_run,data);
        Thread_id_list.push_back(id);
    }    
}

CThreadPll* CThreadPll::GetPoll(int maxCount)
{
    static CThreadPll _thread_pool(maxCount);
    return &_thread_pool;
}

void *CThreadPll::pthread_run(void *pdata)
{
    DataInfo_t *data=(DataInfo_t *)pdata;
    if(data->is_temp_thread==true)
    {
        pthread_detach(pthread_self());
        printf("creat a temp thread id = %d\n",pthread_self());
        data->temp_function();
        CThreadPll::GetPoll()->temp_count_--;
        delete data;
        pthread_exit(NULL);
        return NULL;
    }
    else
    {
        printf("creat thread id = %d\n",pthread_self());
        delete data;
    }
    while(true)
    {
        pthread_mutex_lock(&(CThreadPll::GetPoll()->queue_lock));
        while(CThreadPll::GetPoll()->Task_queue.empty()&&CThreadPll::GetPoll()->is_shut_down!=true)
        {
            CThreadPll::GetPoll()->wait_count_++;
            pthread_cond_wait(&CThreadPll::GetPoll()->cond_t,&CThreadPll::GetPoll()->queue_lock);    
            CThreadPll::GetPoll()->wait_count_--;       
        }
        if(CThreadPll::GetPoll()->is_shut_down)
        {
            pthread_mutex_unlock(&(CThreadPll::GetPoll()->queue_lock));
            printf("pthread %d is exit\n",pthread_self());
            pthread_exit(NULL);
        }
        printf("thread %d ->>",pthread_self());
        CThreadPll::FUNCTION run=CThreadPll::GetPoll()->Task_queue.front();
        CThreadPll::GetPoll()->Task_queue.pop_front();      
        pthread_mutex_unlock(&(CThreadPll::GetPoll()->queue_lock));
        run();
    }
    pthread_exit(NULL);
}
void CThreadPll::addTask(FUNCTION && pTask)
{
    pthread_mutex_lock(&(this->queue_lock));
    if(wait_count_==0)//没有可等待的任务 添加新线程
    {
        if(this->max_temp_count_==this->temp_count_)
        {
            Debug("--------到达最大临时进程-------");
        }
        else
        {
            DataInfo_t *data= new DataInfo_t;
            data->is_temp_thread=true;
            data->temp_function=pTask;
            this->temp_count_++;       
           // printf("temp count ------%d------\n",this->temp_count_);
            pthread_create(&Thread_id_list.back(),NULL,CThreadPll::pthread_run,data);
        }
        pthread_mutex_unlock(&(this->queue_lock));
        return ;
    }
    else
    {
        this->Task_queue.push_back(pTask);//添加任务队列
    } 
    
    pthread_mutex_unlock(&(this->queue_lock));
    if(pthread_cond_signal(&(this->cond_t))<0)
    {
        perror("cond signal");
    }
}
