#ifndef CTCPSOCKET_H_INCLUDED
#define CTCPSOCKET_H_INCLUDED
#include "CBaseSocket.h"
class CTcpSocket:public CBaseSocket
{
public:
    CTcpSocket();
    CTcpSocket(int type, int domain = PF_INET, int protocol = 0);
    HostAddress getRemoteAddr();
    void setRemoteAddr(HostAddress remote_addr);
    int  Connect(HostAddress &remote_addr);
    int  Read(char *buf, int buf_len);
    int  Write(char *buf, int buf_len);
private:
    HostAddress remote_addr;    //保存服务器的ip和端口
};


#endif // CTCPSOCKET_H_INCLUDED
