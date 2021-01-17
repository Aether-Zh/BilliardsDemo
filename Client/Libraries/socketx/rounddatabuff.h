/******************************************************************\
 单向循环字节缓冲区类
 文件：	rounddatabuff.h/cpp
 创建：
 日期：	2012-12-01
 环境：	x86、x64、Windows、Linux
 说明：	可以循环使用的单向字节缓冲区，添加和取出数据时不会移动内存，
 并在需要的时候自动扩大内存从而提性能。适用于顺序追加和取出数据，
 数据只会追加到缓冲区尾部，并只从缓冲区的头部顺序取出。
 \******************************************************************/
#ifndef __ROUNDDATABUFF_H_2012_11_30_11_01___
#define __ROUNDDATABUFF_H_2012_11_30_11_01___

//剩余空间信息
typedef struct _tagUNUSEBUFFINFO
{
	int nPrefix;	//前部剩余空间大小
	int nSuffix;	//后部剩余空间大小
	int nTotal;		//所有剩余空间大小
}
UNUSEBUFFINFO, *PUNUSEBUFFINFO;

//循环缓冲区类
class CRoundDataBuff
{
public:
	CRoundDataBuff(void);
	~CRoundDataBuff(void);
    
public:
	//将数据追加到缓冲区尾部
	bool AddData(unsigned char* pData, int nSize);
    
	//从缓冲区头部顺序取出数据，返回取出数据长度
	int GetData(unsigned char* pData, int nSize);
    
	//从缓冲区头部复制出数据不会将数据取出，返回复制出数据的长度
	int PeekData(unsigned char* pData, int nSize);
    
	//获得缓冲区内数据大小
	int GetCount();
    
	//清空缓冲区内所有数据
	void Empty();
    
	//判断缓冲区是否为空
	bool IsEmpty();
    
	//释放多余的内存空间，返回实际剩余空间大小
	int FreeExtra();
    
	//获得缓冲区空间大小
	int GetBuffSize();
    
	//设置缓冲区最大可分配空间返回之前值，-1为无上限，默认-1
	int SetBuffMaxLimite(int nMax);
    
protected:
	//计算剩余空间
	void CalcFreeBuff(UNUSEBUFFINFO& ubi);
    
	//开辟新空间,nSize为新增加的空间大小
	bool ReallocBuff(int nSize);
    
protected:
	int		m_nMaxSize;		//缓冲区最大分配空间限制
	unsigned char*	m_pBuf;	//缓冲区指针
    
	int		m_nSize;		//缓冲区大小
	int		m_nData;		//所存数据大小
	int		m_nHeadPos;	//buff头位置
	int		m_nTailPos;	//buff尾位置
};

#endif //__ROUNDDATABUFF_H_2012_11_30_11_01___