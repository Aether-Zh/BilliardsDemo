 #include "ccgamesocket.h"
//______________________________________________________________________________________
#define	MAX_PKGSIZE						(64 * 1024)		//最大消息包长度
//______________________________________________________________________________________
CCGameSocket::CCGameSocket(void)
{
}
//______________________________________________________________________________________
CCGameSocket::~CCGameSocket(void)
{
}
//______________________________________________________________________________________
void CCGameSocket::OnConnected(bool bConnected)
{
	CCAsyncSocketX::OnConnected(bConnected);
}
//______________________________________________________________________________________
void CCGameSocket::OnClose()
{
	CCAsyncSocketX::OnClose();
}
//______________________________________________________________________________________
void CCGameSocket::OnReceive(char* pData, int nLen)
{
	CCAsyncSocketX::OnReceive(pData, nLen);
    
	while(!m_RecvBuff.IsEmpty())
	{
		int nBufCnt = m_RecvBuff.GetCount();
		if(nBufCnt <= 0) break;
		//一个整头到达
        int nHdrSize = sizeof(MSGHEADER);
		if(nBufCnt >= nHdrSize)
		{
			MSGHEADER hdr;
			m_RecvBuff.PeekData((unsigned char*)&hdr, nHdrSize);
			int nPkgSize = nHdrSize + hdr.dwLength;
			//超大的包直接扔掉
			if(nPkgSize > MAX_PKGSIZE)
			{
				m_RecvBuff.Empty();
				break;
			}
			if(nBufCnt >= nPkgSize)//一个整包到达，取出数据
			{
				unsigned char* pPkgData = new unsigned char[nPkgSize];
                if(pPkgData == 0) break;
                m_RecvBuff.GetData(pPkgData, nPkgSize);
                PMSGHEADER pHdr = (PMSGHEADER)pPkgData;
                unsigned char* pBdy = pPkgData + nHdrSize;
				OnProcessMsg(pHdr, pBdy, nPkgSize - nHdrSize);
				delete[] pPkgData;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
}
//______________________________________________________________________________________
void CCGameSocket::SendMsg(Message *pMsg, unsigned int dwType, unsigned int dwSeqID/*=0*/)
{
	//消息序列化和组装
	int nSize = 0;
	int nAlloc = 0;
	if(pMsg != 0)
	{
		nSize = pMsg->ByteSize();
		nAlloc = sizeof(MSGHEADER) + nSize;
	}
	else//没有消息体的消息
	{
		nSize = 0;
		nAlloc = sizeof(MSGHEADER);
	}
	char* pBuf = new char[nAlloc];
	PMSGHEADER pReq = (PMSGHEADER)pBuf;
	pReq->dwType = dwType;
	pReq->dwLength = nSize;
	pReq->dwSeqID = dwSeqID;
	if(pMsg != nullptr)
	{
		void* pReqBody = (void*)(pReq + 1);
		if(!pMsg->SerializeToArray((void*)pReqBody, nSize))
		{
			delete[] pBuf;
			return;
		}
	}
    CCAsyncSocketX::Send(pBuf, nAlloc);
	delete[] pBuf;
}