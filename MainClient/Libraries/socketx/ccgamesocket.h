#ifndef __CGAMESOCKET_H_2013_01_19_21_46___
#define __CGAMESOCKET_H_2013_01_19_21_46___

#include "ccasyncsocketx.h"
#include <google/protobuf/message.h>

using namespace std;
using namespace google::protobuf;

//---------------------------------------------------------------------------
//	标准网络通信头结构定义
//---------------------------------------------------------------------------
typedef struct tagMSGHEADER
{
	unsigned int dwType;		//消息操作类型及消息值
	unsigned int dwLength;		//消息头结构后跟的消息体字节长度，不包括头本身长度
	unsigned int dwSeqID;		//请求方标示的消息序列号，接收方不允许修改
}
MSGHEADER, *PMSGHEADER;

//可处理网络数据包的异步Socket类，支持跨平台编译
class CCGameSocket : public CCAsyncSocketX
{
public:
	CCGameSocket(void);
	virtual ~CCGameSocket(void);
    
public:
    virtual void OnConnected(bool bConnected);
	virtual void OnClose();
    
    void OnReceive(char* pData, int nLen);
    
public:
	void SendMsg(Message *pMsg, unsigned int dwType, unsigned int dwSeqID = 0); //发送protobuf消息
    
protected:
    virtual void OnProcessMsg(PMSGHEADER pHdr, unsigned char* pBdy, int nBdyLen) = 0;
};

#endif //__CGAMESOCKET_H_2013_01_19_21_46___