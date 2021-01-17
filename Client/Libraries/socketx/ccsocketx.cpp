#include <stdio.h>
#include <errno.h>
#include "ccsocketx.h"
//______________________________________________________________________________________________
#ifdef WIN32
#pragma comment(lib, "wsock32")
#endif
//______________________________________________________________________________________________
CCSocketX::CCSocketX(SOCKET sock)
{
	m_sock = sock;
}
//______________________________________________________________________________________________
CCSocketX::~CCSocketX()
{
}
//______________________________________________________________________________________________
//只有WIN32才需要的函数
int CCSocketX::Init()
{
#ifdef WIN32
	WSADATA wsaData;
	unsigned short version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData);
	if (ret != 0)
    {
        return -1;
    }
#endif
	return 0;
}
//______________________________________________________________________________________________
//只有WIN32才需要的函数
int CCSocketX::Clean()
{
#ifdef WIN32
	return WSACleanup();
#endif
	return 0;
}
//______________________________________________________________________________________________
CCSocketX& CCSocketX::operator = (SOCKET s)
{
	m_sock = s;
	return (*this);
}
//______________________________________________________________________________________________
CCSocketX::operator SOCKET ()
{
	return m_sock;
}
//______________________________________________________________________________________________
bool CCSocketX::Connect(const char* ip, unsigned short port)
{
	if (m_sock != INVALID_SOCKET)
	{
		Close();
	}
    
    struct addrinfo hints, *res, *res0;
    int error;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    char servename[16] = {0};
    sprintf(servename, "%d", port);
    
    error = getaddrinfo(ip, servename, &hints, &res0);
    if (error != 0)
    {
        return false;
    }
    
    for (res = res0; res; res = res->ai_next)
    {
        // 针对getaddrinfo的servname参数传递端口时，在IPv6下会置为0的解决方案
        switch (res->ai_family)
        {
            case AF_INET:
                reinterpret_cast<struct sockaddr_in *>(res->ai_addr)->sin_port = htons(port);
                break;
            case AF_INET6:
                reinterpret_cast<struct sockaddr_in6 *>(res->ai_addr)->sin6_port = htons(port);
                break;
        }
        
        m_sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (m_sock == INVALID_SOCKET)
        {
            continue;
        }
        
        int ret = connect(m_sock, res->ai_addr, res->ai_addrlen);
        if (ret == SOCKET_ERROR)
        {
            Close();
            continue;
        }
        
        break;
    }
    
    freeaddrinfo(res0);
    
    return m_sock != INVALID_SOCKET;
}
//______________________________________________________________________________________________
bool CCSocketX::Bind(unsigned short port)
{
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = INADDR_ANY;
	svraddr.sin_port = htons(port);
	int opt =  1;
	if (setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0)
    {
        return false;
    }
	int ret = bind(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));
	if (ret == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}
//______________________________________________________________________________________________
bool CCSocketX::Listen(int backlog)
{
	int ret = listen(m_sock, backlog);
	if (ret == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}
//______________________________________________________________________________________________
bool CCSocketX::Accept(CCSocketX& s, char* fromip)
{
	struct sockaddr_in cliaddr;
	socklen_t addrlen = sizeof(cliaddr);
	SOCKET sock = accept(m_sock, (struct sockaddr*)&cliaddr, &addrlen);
	if (sock == SOCKET_ERROR)
	{
		return false;
	}
	s = sock;
	if (fromip != 0)
    {
        sprintf(fromip, "%s", inet_ntoa(cliaddr.sin_addr));
    }
	return true;
}
//______________________________________________________________________________________________
int CCSocketX::Send(const char* buf, int len, int flags)
{
	int bytes;
	int count = 0;
	while (count < len)
	{
		bytes = (int)send(m_sock, buf + count, len - count, flags);
		if (bytes == -1 || bytes == 0)
        {
            return -1;
        }
		count += bytes;
	}
	return count;
}
//______________________________________________________________________________________________
int CCSocketX::Recv(char* buf, int len, int flags)
{
    return (int)recv(m_sock, buf, len, flags);
}
//______________________________________________________________________________________________
int CCSocketX::Close()
{
	int ret = 0;
#ifdef WIN32
	ret = closesocket(m_sock);
#else
	ret = close(m_sock);
#endif
    m_sock = INVALID_SOCKET;
	return ret;
}
//______________________________________________________________________________________________
int CCSocketX::GetLastError()
{
#ifdef WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
}
//______________________________________________________________________________________________
SOCKET CCSocketX::GetHandle()
{
    return m_sock;
}
//______________________________________________________________________________________________