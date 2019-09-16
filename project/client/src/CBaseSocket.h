#ifndef CBASESOCKET_H_INCLUDED
#define CBASESOCKET_H_INCLUDED
#include "HostAddress.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#define Debug(fmt, ...) printf("%s:%d %s>" fmt "\n",__FILE__,__LINE__,__FUNCTION__,##__VA_ARGS__)
class CBaseSocket
{
public:
    CBaseSocket();
    CBaseSocket(int type, int domain = PF_INET, int protocol = 0);
    HostAddress getAddress();
    int  CBind(HostAddress host_addr);
    int  getSockFd();      //»ñÈ¡Ì×½Ó×ÖÃèÊö·û
    void setSockFd(int sock_fd);
    int  closeFd();
protected:
    void create(int domain, int type, int protocol);
private:
    int sock_fd;
};


#endif // CBASESOCKET_H_INCLUDED
