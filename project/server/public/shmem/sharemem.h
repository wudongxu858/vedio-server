#ifndef _SHARE_MEM_H
#define _SHARE_MEM_H
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <map>
#include"../ctool.h"
using namespace std;

class ShareMem
{
private:
    bool *ReadList;
public:
#define FLAG_BIT_MAX 64  //八个字节 64 位
#define FLAG_SIZE sizeof(u_int64_t)
    explicit ShareMem(key_t sharekey,int size,int block_num);
    ~ShareMem();
    template<typename T>
    T GetBlockAddr(int index,int ppos=0)
    {
        T temp =(T)((((char *)share_addr)+this->size*index+ppos+FLAG_SIZE/*+sizeof(bool)*block_num*/));
        return temp;
    }
    template<typename T>
    bool SetBolck(int index,int ppos, const T& data,int size=sizeof(T))
    {
        memcpy(((char*)share_addr)+this->size*index+ppos+FLAG_SIZE/*+sizeof(bool)*block_num*/,(char *)(&data),size);
        return true;
    }
    void ClrBolck(int index)
    {
        memset(((char *)share_addr)+this->size*index+FLAG_SIZE/*+sizeof(bool)*block_num*/,0,this->size);
       
    }
    /*
    *函数:int SetBitFlag(bool IsZero)
    *功能:获取需要是否为零的位
    *bool 需要获得是零或是一的位
    *返回值:位数
    */
    void SetBitFlag(int bit,bool IsZero=true)//默认清零
    {
       //Debug("bit =%d long int =%x & = %x",bit,(*(u_int64_t *)(share_addr)),~(1<<bit));
        if(IsZero==true)
        {
            (*(u_int64_t *)(share_addr))&=(~(1<<bit));//清零
            ReadList[bit]=false;
        }
        else if(IsZero==false)
        {
            (*(u_int64_t *)(share_addr))|=(1<<bit);//置一
        }
    }
    /*
    *函数:int GetBitFlag(bool IsZero)
    *功能:获取需要是否为零的位
    *bool 需要获得是零或是一的位
    *返回值:位数 没有可以获取的放回-1
    */
    int GetBitFlag(bool IsZero)//默认获取为0的
    {
        for(int i=0;i<FLAG_BIT_MAX&&i<block_num;i++)
        {
            if(IsZero==false&&( (*(u_int64_t *)(share_addr))&(1<<i) )&&ReadList[i]==false)//判断是否为1
            {
                ReadList[i]=true;
                return i;
            }
            else if(IsZero==true&&(( (*(u_int64_t *)(share_addr))&(1<<i))==0?true:false))//判断是否为0
            {
                return i;
            }
        }
        return -1;
    }
    void ReadOver(int bit)
    {
        SetBitFlag(bit,true);
     //   ClrBolck(bit);
    }
    bool Detache();
    bool Delete();
    key_t GetKey();
    int GetShmId();
    int GetSize()
    {
        return size;
    }
private:
    u_int64_t isOverFlag;
    int block_num;
    int share_id;
    key_t key;
    void *share_addr;
    int size;
    bool isExist;
};

#endif