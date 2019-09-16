#include "CTcpSocket.h"
CTcpSocket::CTcpSocket()
{

}

CTcpSocket::CTcpSocket(int type, int domain, int protocol)
:CBaseSocket(type, domain, protocol)
{

}

HostAddress CTcpSocket::getRemoteAddr()
{
    return remote_addr;
}

void CTcpSocket::setRemoteAddr(HostAddress remote_addr)
{
    this->remote_addr = remote_addr;
}

int CTcpSocket::Connect(HostAddress &remote_addr)
{
    int sockfd = getSockFd();
    struct sockaddr *serv_addr = remote_addr.getAddress();
    socklen_t addrlen = remote_addr.getLen();
    int ret = connect(sockfd, serv_addr, addrlen);
    return ret;
}

int CTcpSocket::Read(char *buf, int buf_len)
{
    int fd = getSockFd();
    int read_count = read(fd, buf, buf_len);
    return read_count;
}

int CTcpSocket::Write(char *buf, int buf_len)
{
    int fd = getSockFd();
    int write_count = write(fd, buf, buf_len);
    return write_count;
}
