#ifndef _CEPOLL_BASE_H
#define _CEPOLL_BASE_H
#include <iostream>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <functional>
typedef struct epoll_event EPOLL_EVENT;
class CEpollBase
{
public:
    typedef std::function<void(int fd,EPOLL_EVENT event)>FUNCTION;
    explicit CEpollBase(int Maxcount=20,int timeout=-1);
    ~CEpollBase(){}
   bool AddEvent(int fd,int events=EPOLLIN);
   bool DeleteEvent(int fd,int events=EPOLLIN);
   bool ModEvent(int fd,int events=EPOLLIN) ;
   int Wait();
   void SetTimeout(int timeout);
   void Start();
   virtual void Run(int re_event);
   virtual void DelEvent(int fd,EPOLL_EVENT event);
   void Stop();
   void InstallDelFun(FUNCTION function);
   
private:
    FUNCTION function_;
    int epoll_fd_;
    EPOLL_EVENT addevent;
    EPOLL_EVENT *events;
    int Maxcount;
    int timeout;
    bool is_stop;  
};

#endif