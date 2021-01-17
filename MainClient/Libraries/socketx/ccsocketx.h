#ifndef __CCSOCKETX_H_2013_01_19_21_05___
#define __CCSOCKETX_H_2013_01_19_21_05___

#ifdef WIN32 ////////////////////////////////////////////////////////////
#include <winsock2.h>
typedef int				socklen_t;
#else ///////////////////////////////////////////////////////////////////
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
typedef int				SOCKET;
#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1
#endif////////////////////////////////////////////////////////////////////

//支持跨平台编译的Socket基础类
class CCSocketX
{
public:
	CCSocketX(SOCKET sock = INVALID_SOCKET);
	~CCSocketX();
    
public:
	static int Init();
	static int Clean();
    
public:
    CCSocketX& operator = (SOCKET s);
    operator SOCKET ();    
	bool Connect(const char* ip, unsigned short port);
	bool Bind(unsigned short port);
	bool Listen(int backlog = 5);
	bool Accept(CCSocketX& s, char* fromip = 0);
	int Send(const char* buf, int len, int flags = 0);
	int Recv(char* buf, int len, int flags = 0);
	int Close();
	int GetLastError();
    SOCKET GetHandle();
    
protected:
	SOCKET m_sock;
};

#endif //__CCSOCKETX_H_2013_01_19_21_05___