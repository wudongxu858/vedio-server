#include "CSemaphore.h"

CSemaphore::CSemaphore(key_t key, int nsems)
:semid(-1)
{
    isExist=false;
    if (sem_create(key, nsems) == false)
    {
        perror("sem_create");
        return ;
    }
}

CSemaphore::~CSemaphore()
{
    
    if(isExist==false)
    {
        sem_delete();
    }
}

bool CSemaphore::sem_create(key_t key, int nsems)
{
    semid=semget(key,nsems,IPC_CREAT|IPC_EXCL);
    if(errno == EEXIST)
    {
        isExist=true;    
    }
    
    semid = semget(key, nsems, IPC_CREAT|0666);
    if (semid < 0)
    {
        perror("sem create error");
        return false;
    }
    return true;
}

/*bool CSemaphore::sem_open(key_t key)
{
    semid = semget(key, 0, 0);
    if (semid < 0)
    {
        perror("sem open error");
        return false;
    }
    return true;
}*/

int CSemaphore::sem_delete()
{
    int ret = semctl(semid, 0, IPC_RMID);
    if (ret < 0)
    {
        perror("sem delete error");
    }
    return ret;
}

int CSemaphore::sem_setval(int nsempos, int val)
{
    union semun arg;
    arg.val = val;
    int ret = semctl(semid, nsempos, SETVAL, arg);
    if (ret < 0)
    {
        perror("set val error");
    }
    return ret;
}

int CSemaphore::sem_getval(int nsempos)
{
    int val = semctl(semid, nsempos, GETVAL);
    if (val < 0)
    {
        perror("get val error");
    }
    return val;
}

int CSemaphore::sem_setall(unsigned short *values)
{
    union semun arg;
    arg.arr = values;
    int ret = semctl(semid, 0, SETALL, values);
    if (ret < 0)
    {
        perror("sem set all error");
    }
    return ret;
}

int CSemaphore::sem_getall(unsigned short *values)
{
    union semun arg;
    arg.arr = values;
    int ret = semctl(semid, 0, GETALL, values);
    if (ret < 0)
    {
        perror("sem set all error");
    }
    return ret;
}

int CSemaphore::sem_p(short unsigned pos)
{
    struct sembuf buf = {pos, -1,0};
    int ret = semop(semid, &buf, 1);
    if (ret < 0)
    {
        perror("sem p error");
    }
    return ret;
}

int CSemaphore::sem_v(short unsigned pos)
{
    struct sembuf buf = {pos, 1, 0};
    int ret = semop(semid, &buf, 1);
    if (ret < 0)
    {
        perror("sem v error");
    }
    return ret;
}
