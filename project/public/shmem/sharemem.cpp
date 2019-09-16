#include "sharemem.h"
ShareMem::~ShareMem()
{
    if(isExist)
    {
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
    isExist=false;
    share_id=shmget(sharekey,size*block_num,IPC_CREAT|IPC_EXCL);
    if(errno==EEXIST)
    {
        isExist=true;
    }
    share_id=shmget(sharekey,size*block_num,IPC_CREAT|0666);
    if(share_id<0)
    {
        perror("shamget");
        return ;
    }
    share_addr=shmat(share_id,0,0);
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