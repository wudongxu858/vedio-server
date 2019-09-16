#ifndef CSEMAPHORE_H_INCLUDED
#define CSEMAPHORE_H_INCLUDED
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
using namespace std;
union semun{
    int             val;
    struct semid_ds *buf;
    unsigned short  *arr;
    struct seminfo  *_buf;
};
class CSemaphore
{
public:
    CSemaphore(key_t key, int nsems);
    ~CSemaphore();
    bool sem_create(key_t key, int nsems);
//    bool sem_open(key_t key);
    int sem_setval(int nsempos, int val);
    int sem_getval(int nsempos);
    int sem_setall(unsigned short *values);
    int sem_getall(unsigned short *values);
    int sem_p(short unsigned pos);
    int sem_v(short unsigned pos);
private:
    int sem_delete();
    int semid;
    bool isExist;
};


#endif // CSEMAPHORE_H_INCLUDED
