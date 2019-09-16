#include "sharemem.h"
ShareMem::~ShareMem()
{
    if(isExist)
    {
        delete [] ReadList;
        this->Detache();
    }
    else
    {
        this->Delete();
    }
}
ShareMem::ShareMem(key_t sharekey,int size,int block_num)
{
    this->block_num=block_num;
    this->share_id=0;
    this->share_addr=NULL;
    this->size=size;
    this->key=sharekey;
    ReadList=new bool[block_num];
    memset(ReadList,0,sizeof(bool)*block_num);
    isOverFlag=0;
    isExist=false;
    share_id=shmget(sharekey,/*sizeof(bool)*block_num+*/size*block_num+FLAG_SIZE,IPC_CREAT|IPC_EXCL);
    if(errno==EEXIST)
    {
        isExist=true;
    }
    share_id=shmget(sharekey,/*sizeof(bool)*block_num+*/size*block_num+FLAG_SIZE,IPC_CREAT|0666);
    if(share_id<0)
    {
        perror("shamget");
        return ;
    }
    share_addr=shmat(share_id,0,0);
    memset(share_addr,0,/*size*block_num+*/FLAG_SIZE+sizeof(bool)*block_num);
    
    
    if(share_addr==0)
    {
        perror("shmat");
        return ;
    }
}
key_t ShareMem::GetKey()
{
    return key;
}
int ShareMem::GetShmId()
{
    return share_id;
}
bool ShareMem::Detache()
{
    if(shmdt(share_addr)==-1)
    {
        perror("shmdt");
        return false;
    }
    return true;
}
bool ShareMem::Delete()
{
    if(shmctl(share_id, IPC_RMID, 0) == -1)
    {
        perror("shmctl");
        return false;
    }
    return true;
}