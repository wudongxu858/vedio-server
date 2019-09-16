#include "CBaseSocket.h"

CBaseSocket::CBaseSocket()
{
    sock_fd = 0;
}

CBaseSocket::CBaseSocket(int type, int domain, int protocol)
{
    create(domain, type, protocol);
}

HostAddress CBaseSocket::getAddress()
{
    HostAddress addr;
    memset(&addr, 0, sizeof(addr));
    getsockname(sock_fd, addr.getAddress(), addr.getLenPtr());
    return addr;
}

int CBaseSocket::CBind(HostAddress host_addr)
{
    int mw_optval=1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&mw_optval, sizeof(mw_optval));
    int ret = bind(sock_fd, host_addr.getAddress(), host_addr.getLen());
  /*  if(ret < 0)
    {
        perror("bind");
    }*/
    return ret;
}

int CBaseSocket::getSockFd()
{
    return sock_fd;
}

void CBaseSocket::setSockFd(int sock_fd)
{
    this->sock_fd = sock_fd;
}

int CBaseSocket::closeFd()
{
    return close(sock_fd);
}

void CBaseSocket::create(int domain, int type, int protocol)
{
    sock_fd = socket(domain, type, protocol);
}
