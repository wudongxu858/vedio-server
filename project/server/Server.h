#ifndef _SERVER_H_
#define _SERVER_H_
#include <iostream>
#include <signal.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <array>
#include <sqlite3.h>
#include "./public/socket/HostAddress.h"
#include "./public/socket/CTcpSocket.h"
#include "./public/socket/CSerSocket.h"
#include "./public/cepollbase/cepollbase.h"
#include "./public/thread/threadpool.h"
#include "./public/csem/CSemaphore.h"
#include "./public/shmem/sharemem.h"
#include "./public/pack/pack.h"
#include "./public/sqlite/mysqlite.h"
using namespace std;
class Base
{
public:
    typedef struct send_Post
    {
        int type;
        int size;
        int fd;
    }SEND_POST;
    Base(key_t cem_key,key_t sharemem_key,key_t memback_key)//信号 放入内存 取出内存
    :cem_key(cem_key),sem(cem_key,2),sharemem_key(sharemem_key),memback_key(memback_key)
    ,shmem(sharemem_key,100,20),backshmem(memback_key,100,20){}
    virtual void Send_Server()=0;
    virtual void Recv_Server()=0;
    virtual void Run() =0;
    void MemGetData(SEND_POST pos,char *buff,int bit,ShareMem &mem);
protected:
    key_t cem_key;//信号键
    CSemaphore sem;//信号
    key_t sharemem_key;
    ShareMem shmem;//放入后置共享内存
    key_t memback_key;
    ShareMem backshmem;//后置放入前置共享内存
    CThreadPll * threadpool;
};
class PostServer :public Base
{
private:
    SEND_POST pos;
public:
    PostServer(key_t cem_key,key_t sharemem_key,key_t memback_key);
    virtual void Send_Server() override;
    virtual void Recv_Server() override;
    virtual void Run() override;
    
};
class PreServer :public Base
{
private:
    CSerSocket server;
    CTcpSocket client;
    CEpollBase epoll;
    HostAddress ser_addr;
    pthread_mutex_t send_mutex;
    char rev_buff[1024];
public:
    PreServer(key_t cem_key,key_t sharemem_key,key_t memback_key);
    virtual void Run() override;
    virtual void Recv_Server() override;
    virtual void Send_Server();
    void DelPack(PreServer* self,int size,int fd,char *data);
};
#endif