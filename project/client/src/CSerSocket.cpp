#include "CSerSocket.h"
CSerSocket::CSerSocket()
{

}

CSerSocket::CSerSocket(int type, int domain, int protocol)
:CBaseSocket(type, domain, protocol)
{

}

bool CSerSocket::Listen(int backLog)
{
    int fd = getSockFd();
    if (listen(fd, backLog) == -1)
    {
        perror("listen");
        return false;
    }
    return true;
}

CTcpSocket CSerSocket::Accept()
{
    CTcpSocket client_socket;
    HostAddress client_addr;
    int server_fd = getSockFd();
    int accept_socket = accept(server_fd, client_addr.getAddress(), client_addr.getLenPtr());
    client_socket.setSockFd(accept_socket);
    client_socket.setRemoteAddr(client_addr);
    return client_socket;
}

void CSerSocket::StartServer(HostAddress &ser_addr, int backLog)
{
    if (CBind(ser_addr) < 0)
    {
        return ;
    }
    if (Listen(backLog) == false)
    {
        return ;
    }
    CTcpSocket *client_socket;
    while(1)
    {
        client_socket = new CTcpSocket(SOCK_STREAM);
        *client_socket = Accept();

        if( pthread_create(&thread_id, NULL, thread_fun, client_socket) < 0)
        {
            perror("create thread");
            return ;
        }
    }
}

void *CSerSocket::thread_fun(void *p)
{
    CTcpSocket *client_socket = (CTcpSocket *)p;
    char buffer[50]={0};
    int read_count=0, write_count=0;
    while (1)
    {
        read_count = client_socket->Read(buffer, sizeof(buffer));
        write_count = client_socket->Write(buffer, strlen(buffer));
        memset(buffer, 0, sizeof(buffer));
    }
}

CSerSocket::~CSerSocket()
{

}
