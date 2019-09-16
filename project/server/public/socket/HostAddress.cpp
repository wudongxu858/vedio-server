#include "HostAddress.h"
HostAddress::HostAddress(unsigned short port, char *ip)
{
    length = sizeof(m_addr);
    memset(&m_addr, 0, length);
    if(port > 1024 && port < 65535)
    {
        m_addr.sin_port = htons(port);
    }
    else
    {
        m_addr.sin_port = htons(1234);
    }
    if (ip == NULL)
    {
        m_addr.sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        m_addr.sin_addr.s_addr = inet_addr(ip);
    }
    m_addr.sin_family = PF_INET;
}

HostAddress::HostAddress()
{
    length = sizeof(m_addr);
    memset(&m_addr, 0, length);
    m_addr.sin_family = PF_INET;
}

void HostAddress::setIp(char *ip)
{
    m_addr.sin_addr.s_addr = inet_addr(ip);
}

void HostAddress::setPort(unsigned short port)
{
    m_addr.sin_port = htons(port);
}

char *HostAddress::getIp()
{
    return inet_ntoa(m_addr.sin_addr);
}

unsigned short HostAddress::getPort()
{
    return ntohs(m_addr.sin_port);
}

struct sockaddr *HostAddress::getAddress()
{
    return (struct sockaddr *)&m_addr;
}

socklen_t HostAddress::getLen()
{
    return length;
}

socklen_t *HostAddress::getLenPtr()
{
    return (socklen_t *)&length;
}

HostAddress::~HostAddress()
{

}
