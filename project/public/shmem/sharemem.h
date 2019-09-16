#ifndef _SHARE_MEM_H
#define _SHARE_MEM_H
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
using namespace std;

class ShareMem
{
public:
    explicit ShareMem(key_t sharekey,int size,int block_num);
    ~ShareMem();
    template<typename T>
    T GetBlockAddr(int index)
    {
        return (T)(((char *)share_addr)+this->size*index);
    }
    template<typename T>
    bool SetBolck(int index, const T& data)
    {
        memcpy(((char*)share_addr)+this->size*index,&data,sizeof(T));
        return true;
    }
    void ClrBolck(int index)
    {
        memset(((char *)share_addr)+this->size*index,0,this->size);
    }
    bool Detache();
    bool Delete();
    key_t GetKey();
    int GetShmId();
private:
    int block_num;
    int share_id;
    key_t key;
    void *share_addr;
    int size;
    bool isExist;
};

#endif