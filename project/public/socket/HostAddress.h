#ifndef HOSTADDRESS_H_INCLUDED
#define HOSTADDRESS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
class HostAddress
{
public:
    HostAddress();
    HostAddress(unsigned short port, char *ip = NULL);
    ~HostAddress();
    void setIp(char *ip);
    void setPort(unsigned short port);
    char *getIp();
    unsigned short   getPort();
    struct sockaddr *getAddress();
    socklen_t  getLen();
    socklen_t *getLenPtr();
private:
    struct sockaddr_in m_addr;
    int length;
};


#endif // HOSTADDRESS_H_INCLUDED
