#include <stdio.h>
#include "ccasyncsocketx.h"

#define RECV_BUFF_SIZE	(4*1024)
//______________________________________________________________________________________________
#ifdef WIN32 //////////////////////////////////////////////

#else /////////////////////////////////////////////////////

#include <sys/time.h>
#include <signal.h>
#include <string.h>

#endif ////////////////////////////////////////////////////
//______________________________________________________________________________________________
CCAsyncSocketX::CCAsyncSocketX(void)
{
    m_nState = SOCKET_STATE_IDLE;
    m_pContext = 0;
    m_hThread = 0;
	memset(m_szIP, 0, sizeof(m_szIP));
	m_nPort = 0;
}
//______________________________________________________________________________________________
CCAsyncSocketX::~CCAsyncSocketX(void)
{
	Close();
}
//______________________________________________________________________________________________
void CCAsyncSocketX::Connect(const char* szIP, unsigned short nPort)
{
    if (m_nState != SOCKET_STATE_IDLE)
    {
        Close();
    }
    strncpy(m_szIP, szIP, sizeof(m_szIP) - 1);
    m_nPort = nPort;
    m_nState = SOCKET_STATE_CONNECTING;
    m_pContext = CCAsyncSocketXWorkerThreadContext::Create(this, this, szIP, nPort);
#ifdef WIN32 ////////////////////////////////////////////////////////////////
    //创建接收线程
    unsigned long dwID = 0;
    m_hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)_recvfunc, (void*)m_pContext, CREATE_SUSPENDED, &dwID);
    //设置优先级为普通
    SetThreadPriority(m_hThread, THREAD_PRIORITY_NORMAL);
    //启动线程
    ResumeThread(m_hThread);
#else ////////////////////////////////////////////////////////////////////////
    //忽略信号
    signal(SIGPIPE, SIG_IGN);
    //创建接收线程
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&m_hThread, &attr, _recvfunc, (void*)m_pContext);
#endif ///////////////////////////////////////////////////////////////////////
}
//______________________________________________________________________________________________
void CCAsyncSocketX::Send(const char *szBuff, int nLen)
{
    CCSocketX::Send(szBuff, nLen);
}
//______________________________________________________________________________________________
void CCAsyncSocketX::Close()
{
    if (m_nState == SOCKET_STATE_IDLE)
    {
        return;
    }
    //终止线程
#ifdef WIN32 ////////////////////////////////////////////////////////////
    unsigned long dwRet = WaitForSingleObject(m_hThread, 5000);
    if (dwRet == WAIT_TIMEOUT)
    {
        TerminateThread(m_hThread, (unsigned long)-1);
    }
    CloseHandle(m_hThread);
#else ////////////////////////////////////////////////////////////////////
    pthread_detach(m_hThread);
#endif ///////////////////////////////////////////////////////////////////
    Reset();
}
//______________________________________________________________________________________________
void CCAsyncSocketX::OnClose()
{
    Reset();
}
//______________________________________________________________________________________________
void CCAsyncSocketX::OnConnected(bool bConnected)
{
    if (bConnected)
    {
        m_nState = SOCKET_STATE_CONNECTED;
    }
    else
    {
        Reset();
    }
}
//______________________________________________________________________________________________
void CCAsyncSocketX::OnReceive(char* pData, int nLen)
{
    m_RecvBuff.AddData((unsigned char*)pData, nLen);
}
//______________________________________________________________________________________________
void CCAsyncSocketX::Reset()
{
    m_nState = SOCKET_STATE_IDLE;
    m_hThread = 0;
    if (m_pContext)
    {
        m_pContext->Release();
        m_pContext = 0;
    }
    CCSocketX::Close();
}
//______________________________________________________________________________________________
void* CCAsyncSocketX::_recvfunc(void* pParam)
{
    CCAsyncSocketXWorkerThreadContext* pContext = static_cast<CCAsyncSocketXWorkerThreadContext*>(pParam);
    
    do
    {
        if (!pContext->IsWorking())
        {
            break;
        }
        //建立连接
        bool bRet = pContext->GetSocket()->Connect(pContext->GetIP(), pContext->GetPort());
        if (!pContext->IsWorking())
        {
            break;
        }
        pContext->GetProxy()->OnConnected(bRet);
        if (!bRet)
        {
            break;
        }
        //接收数据
        SOCKET handle = pContext->GetSocket()->GetHandle();
        fd_set set;
        timeval tm;
        char szBuff[RECV_BUFF_SIZE] = {0};
        while (pContext->IsWorking())
        {
            FD_ZERO(&set);
            FD_SET(handle, &set);
            tm.tv_sec = 0;//秒
            tm.tv_usec = 100 * 1000;//微秒
            int nRet = select(handle + 1, &set, 0, 0, &tm);
            if (nRet > 0)
            {
                //有数据
                if (FD_ISSET(handle, &set))
                {
                    memset(szBuff, 0, sizeof(szBuff));
                    int nRecv = pContext->GetSocket()->Recv(szBuff, RECV_BUFF_SIZE);
                    if (nRecv > 0)
                    {
                        //收到数据
                        pContext->GetProxy()->OnReceive(szBuff, nRecv);
                    }
                    else
                    {
                        //远端断开连接了
                        if (pContext->GetSocket()->Close() == 0)
                        {
                            pContext->GetProxy()->OnClose();
                        }
                        break;
                    }
                }
            }
            else if (nRet < 0)
            {
                //出错了
                if (pContext->GetSocket()->Close() == 0)
                {
                    pContext->GetProxy()->OnClose();
                }
                break;
            }
            else
            {
                //无数据
                continue;
            }
        }
    } while (0);
    
    delete pContext;
    
    return 0;
}
//______________________________________________________________________________________________
CCAsyncSocketXWorkerThreadContext* CCAsyncSocketXWorkerThreadContext::Create(CCAsyncSocketX* pProxy,
                                                                             CCSocketX* pSocket,
                                                                             const char* szIP,
                                                                             unsigned short nPort)
{
    return new CCAsyncSocketXWorkerThreadContext(pProxy, pSocket, szIP, nPort);
}
//______________________________________________________________________________________________
CCAsyncSocketXWorkerThreadContext::CCAsyncSocketXWorkerThreadContext(CCAsyncSocketX* pProxy,
                                                                     CCSocketX* pSocket,
                                                                     const char* szIP,
                                                                     unsigned short nPort)
{
    m_pProxy = pProxy;
    m_pSocket = pSocket;
    strncpy(m_szIP, szIP, sizeof(m_szIP) - 1);
    m_nPort = nPort;
    m_bReleased = false;
}
//______________________________________________________________________________________________
CCAsyncSocketXWorkerThreadContext::~CCAsyncSocketXWorkerThreadContext()
{
}
//______________________________________________________________________________________________
void CCAsyncSocketXWorkerThreadContext::Release()
{
    m_bReleased = true;
}
//______________________________________________________________________________________________
