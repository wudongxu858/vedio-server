#include"cepollbase.h"
#define Debug(fmt, ...) printf("%s:%d %s>" fmt "\n",__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)
CEpollBase::CEpollBase(int Maxcount,int timeout)
:Maxcount(Maxcount),timeout(timeout)
{
    this->function_=NULL;
    this->is_stop=false;
    this->epoll_fd_=epoll_create(this->Maxcount);
    if(this->epoll_fd_<0)
    {
        perror("EPOLL_CREATE");
        exit(0);
    }
    events=new EPOLL_EVENT[this->Maxcount];
    if(events==NULL)
    {
        perror("new events error");
        exit(0);
    }
}
bool CEpollBase::AddEvent(int fd,int events)
{
    Debug("ADD OK");
    if(this->epoll_fd_<0)
    {
        perror("epoll_fd_");
        return false;
    }
    memset(&addevent,0,sizeof(EPOLL_EVENT));
    addevent.events=events;
    addevent.data.fd=fd;
    int res=epoll_ctl(this->epoll_fd_,EPOLL_CTL_ADD,fd,&addevent);
    if(res<0)
    {
        perror("EPOLL_CTL_ADD");
        return false;
    }
    return true;
}
bool CEpollBase::DeleteEvent(int fd,int events)
{
    
    if(this->epoll_fd_<0)
    {
        perror("epoll_fd_");
        return false;
    }
    memset(&addevent,0,sizeof(EPOLL_EVENT));
    addevent.events=events;
    addevent.data.fd=fd;
    int res=epoll_ctl(this->epoll_fd_,EPOLL_CTL_DEL,fd,NULL);
    if(res<0)
    {
        perror("EPOLL_CTL_DEL");
        return false;
    }
    return true;
}
bool CEpollBase::ModEvent(int fd,int events)
{
    if(this->epoll_fd_<0)
    {
        perror("epoll_fd_");
        return false;
    }
    memset(&addevent,0,sizeof(EPOLL_EVENT));
    addevent.events=events;
    addevent.data.fd=fd;
    int res=epoll_ctl(this->epoll_fd_,EPOLL_CTL_MOD,fd,&addevent);
    if(res<0)
    {
        perror("EPOLL_CTL_MOD");
        return false;
    }
    return true;
}

int CEpollBase::Wait()
{
    int rev_events=epoll_wait(this->epoll_fd_,this->events,this->Maxcount,this->timeout);
    
    if(rev_events==-1)
    {
        perror("epoll wait..");
        return -1;
    }
    else if(rev_events==0)
    {
        perror("time out ..");
        return 0;
    }
    return rev_events;
}
void CEpollBase::SetTimeout(int timeout)
{
    this->timeout=timeout;
}
void CEpollBase::Run(int re_event)
{
    for(int i=0;i<re_event;i++)
    {
        this->DelEvent((this->events+i)->data.fd,*(this->events+i));
    }
}
void CEpollBase::Stop()
{
    is_stop=true;
}
void CEpollBase::Start()
{
    int re_events=0;
    is_stop=true;
    while(!is_stop)
    {
        re_events=Wait();             
        this->Run(re_events);
    }
}
void CEpollBase::DelEvent(int fd,EPOLL_EVENT event)
{
    if(function_!=NULL)
        function_(fd,event);
    else
    {
        Debug("FUNCTION NOT INSTALL");
    }
}
void CEpollBase::InstallDelFun(FUNCTION function)
{
    function_=function;
}