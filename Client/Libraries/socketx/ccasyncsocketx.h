#ifndef __CCASYNCSOCKETX_H_2013_01_19_21_45___
#define __CCASYNCSOCKETX_H_2013_01_19_21_45___

#include "ccsocketx.h"
#include "rounddatabuff.h"

#ifdef WIN32 ///////////////////////////////////////

#else //////////////////////////////////////////////

#include <pthread.h>
typedef pthread_t		HANDLE;

#endif /////////////////////////////////////////////

//由于可能使用域名，所以不能只有16个的IP长度
#define MAX_IP_LEN              256

//Socket状态
#define SOCKET_STATE_IDLE       0
#define SOCKET_STATE_CONNECTING 1
#define SOCKET_STATE_CONNECTED  2

class CCAsyncSocketXWorkerThreadContext;

//支持跨平台编译的异步Socket基础类
class CCAsyncSocketX : public CCSocketX
{
public:
	CCAsyncSocketX(void);
	virtual ~CCAsyncSocketX(void);
    
public:
	void Connect(const char* szIP, unsigned short nPort);
    void Send(const char* szBuff, int nLen);
	void Close();
    
    virtual void OnConnected(bool bConnected);
	virtual void OnClose();
	virtual void OnReceive(char* pData, int nLen);
    
public:
    inline bool IsIdle() { return m_nState == SOCKET_STATE_IDLE; }
    inline bool IsConnecting() { return m_nState == SOCKET_STATE_CONNECTING; }
    inline bool IsConnected() {return m_nState == SOCKET_STATE_CONNECTED; }
    
private:
    void Reset();
    
private:
	static void* _recvfunc(void* pParam);
    
protected:
    int                                m_nState;
    CCAsyncSocketXWorkerThreadContext* m_pContext;
	HANDLE                             m_hThread;
    
protected:
    CRoundDataBuff	m_RecvBuff;
    
public:
	char m_szIP[MAX_IP_LEN];
	unsigned short m_nPort;
};

class CCAsyncSocketXWorkerThreadContext
{
public:
    static CCAsyncSocketXWorkerThreadContext* Create(CCAsyncSocketX* pProxy,
                                                     CCSocketX* pSocket,
                                                     const char* szIP,
                                                     unsigned short nPort);
    
public:
    CCAsyncSocketXWorkerThreadContext(CCAsyncSocketX* pProxy,
                                      CCSocketX* pSocket,
                                      const char* szIP,
                                      unsigned short nPort);
    ~CCAsyncSocketXWorkerThreadContext();
    
    void Release();
    
    inline bool IsWorking() { return !m_bReleased; }
    inline CCAsyncSocketX* GetProxy() { return m_pProxy; }
    inline CCSocketX* GetSocket() { return m_pSocket; }
    inline const char* GetIP() { return m_szIP; }
    inline unsigned short GetPort() { return m_nPort; }
    
private:
    CCAsyncSocketX* m_pProxy;
    CCSocketX* m_pSocket;
    char m_szIP[MAX_IP_LEN];
    unsigned short m_nPort;
    bool m_bReleased;
};

#endif //__CCASYNCSOCKETX_H_2013_01_19_21_45___