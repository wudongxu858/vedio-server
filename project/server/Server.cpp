#include "Server.h"

PostServer::PostServer(key_t cem_key,key_t sharemem_key,key_t memback_key)
    :Base(cem_key,sharemem_key,memback_key)
{
    threadpool=CThreadPll::GetPoll(10);
}


void PostServer::Run()
{
    Recv_Server();
}
void PostServer::Send_Server()
{
   
}
/********************************PRESEVER*******************************************************/

PreServer::PreServer(key_t cem_key,key_t sharemem_key,key_t memback_key)
    :Base(cem_key,sharemem_key,memback_key),server(SOCK_STREAM),client(SOCK_STREAM),epoll(20),ser_addr(9999)
{
   // shmem.ClrBolck(sem.sem_getval(0));
    sem.sem_setval(0,0);//20个资源
    sem.sem_setval(1,0);
    pthread_mutex_init(&send_mutex,NULL);
    threadpool=CThreadPll::GetPoll(20);
    epoll.AddEvent(server.getSockFd());
    server.Ready(ser_addr,20);
    sqlite3 *db=NULL;
    sqlite3_open("./db.db",&db);
}
void PreServer::Run() 
{
    Send_Server();
    Recv_Server();
    epoll.Start();
}

void PreServer::Send_Server() 
{
    epoll.InstallDelFun([this](int fd,EPOLL_EVENT event){
    if(server.getSockFd()==fd)
    {         
        client=server.Accept();
        if(client.getSockFd()==-1)
        {
            Debug("error");
            exit(0);
        }
        Debug("new client=%d",client.getSockFd());
        if(client.getSockFd()==-1)
        {
            return;
        }
        if(!epoll.AddEvent(client.getSockFd(),EPOLLIN))
        {
            perror("EPOLL_CTL_ADD :");
            return ;
        }
    }
    else if(event.events&EPOLLIN)
    {
        memset(rev_buff,0,1024);
        int size=recv(fd,rev_buff,1024,0);
        if(size<=0)//  ==0是关闭  <0是管道断裂
        {
            Debug("client down..");
            epoll.DeleteEvent(fd);
            close(fd);
            return;
        } 
        char *temp=new char[size];
        memcpy(temp,rev_buff,size);
        threadpool->addTask([this,size,fd,temp](){
        //处理包的位置
            DelPack(this,size,fd,temp);
        });
    }
});
}
void PreServer::DelPack(PreServer* self,int size,int fd,char *data)
{
    char *tempdata=data;
    while(1)
    {
        pthread_mutex_lock(&send_mutex);
       // Debug("semcout=%d",sem.sem_getval(0));
        while(sem.sem_getval(0)>=20);//共享内存块不够写入锁死
        PACK_RETURN res=PackDel::getPack(size,tempdata);
        if(res.isOver==true)//结束
        {
            pthread_mutex_unlock(&send_mutex);
            break;
        }
        SEND_POST pos={res.type,res.lenth,fd};
        int bit=shmem.GetBitFlag(true);
       // Debug("send bit =%d",bit);
        shmem.SetBitFlag(bit,false);
        shmem.SetBolck(bit,0,pos);
        shmem.SetBolck(bit,sizeof(SEND_POST),*res.data,res.lenth);
        sem.sem_v(0);//添加可读资源
        pthread_mutex_unlock(&send_mutex);
    }
    delete [] data;   
  //  Debug("OUT");           
    
}

void Base::MemGetData(SEND_POST pos,char *buff,int bit,ShareMem &mem)
{
    memset(buff,0,mem.GetSize());
    memcpy(buff,mem.GetBlockAddr<char*>(bit,sizeof(SEND_POST)),pos.size);
    mem.ReadOver(bit);
}
void PostServer::Recv_Server()
{
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex,NULL);
    while(1)
    {
        sem.sem_p(0);
        int bit=shmem.GetBitFlag(false);
        threadpool->addTask([this,bit,&mutex](){
            SEND_POST pos=*shmem.GetBlockAddr<SEND_POST*>(bit);
            int datalenth=shmem.GetSize();
            int sendlenth=pos.size+PACK_HEAD_SIZE+PACK_TAIL_SIZE;
            char buff[datalenth];
            char sendbuff[sendlenth];
            memset(buff,0,datalenth);
            memset(sendbuff,0,sendlenth);
            MemGetData(pos,buff,bit,shmem);//共享内存读取数据
            shmem.ReadOver(bit);
          //  Debug("type =%d ,data=%s",pos.type,buff);
            switch (pos.type)//不同类型数据的处理
            {
            case 0:
                PackDel::setPack(pos.type,pos.size,buff,sendbuff);
                pos.size=sendlenth;
                Debug("pack size =%d",pos.size);
                break;
            default:
                break;
            }
            pthread_mutex_lock(&mutex);
            int bit=backshmem.GetBitFlag(true);
           // Debug("send bit =%d",bit);
            backshmem.SetBitFlag(bit,false);
            backshmem.SetBolck(bit,0,pos);
            backshmem.SetBolck(bit,sizeof(SEND_POST),*sendbuff,pos.size);
            sem.sem_v(1);//添加可读资源
            pthread_mutex_unlock(&mutex);
        });
    }
}
void PreServer::Recv_Server() //发送数据数据包给客户端
{
    
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex,NULL);
    threadpool->addTask([this,&mutex](){
        Debug("enter");
        while(1)
        {
            sem.sem_p(1);
          //  Debug("get data");
            int bit=backshmem.GetBitFlag(false);
          //  threadpool->addTask([bit,this,&mutex](){
              //  Debug("recive bit =%d",bit);
                SEND_POST pos=*backshmem.GetBlockAddr<SEND_POST*>(bit);
              //  Debug("send clinet =%d",pos.size);
                char buff[backshmem.GetSize()];
                memset(buff,0,backshmem.GetSize());
                MemGetData(pos,buff,bit,backshmem);
               // pthread_mutex_lock(&mutex);
                send(pos.fd,buff,pos.size,0);
                usleep(10);
               // pthread_mutex_unlock(&mutex);
                
           // });   
        }
    });
}
