#ifndef CSERSOCKET_H_INCLUDED
#define CSERSOCKET_H_INCLUDED
#include "CTcpSocket.h"
#include <pthread.h>
class CSerSocket:public CBaseSocket
{
public:
    CSerSocket();
    CSerSocket(int type, int domain = PF_INET, int protocol = 0);
    void StartServer(HostAddress &ser_addr, int backLog);
    static void *thread_fun(void *p);
    ~CSerSocket();
private:
    bool Listen(int backLog);
    CTcpSocket Accept();
    pthread_t  thread_id;
};


#endif // CSERSOCKET_H_INCLUDED
