#include "rounddatabuff.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

//______________________________________________________________________________________
/*#ifdef _DEBUG
 #define ZEROMEM(x,c)	memset((x),0,(c))
 #else
 #define ZEROMEM(x,c)	(x)
 #endif //_DEBUG*/
//______________________________________________________________________________________
CRoundDataBuff::CRoundDataBuff(void)
{
	m_nMaxSize = -1;
	m_pBuf = 0;
	m_nSize = 0;
	m_nData = 0;
	m_nHeadPos = -1;
	m_nTailPos = -1;
}
//______________________________________________________________________________________
CRoundDataBuff::~CRoundDataBuff(void)
{
	if(m_pBuf != 0)
	{
		free(m_pBuf);
		m_pBuf = 0;
	}
	m_nSize = 0;
	m_nData = 0;
	m_nHeadPos = -1;
	m_nTailPos = -1;
}
//______________________________________________________________________________________
//将数据追加到缓冲区尾部
bool CRoundDataBuff::AddData(unsigned char* pData, int nSize)
{
	if(pData==0 || nSize<=0)	return false;
	//计算空间
	UNUSEBUFFINFO ubi;
	CalcFreeBuff(ubi);
	//空间不够
	if(ubi.nTotal < nSize)
	{
		int nAllocSize = nSize - ubi.nTotal;
		if(!ReallocBuff(nAllocSize)) return false;
		//重新计算空间
		CalcFreeBuff(ubi);
	}
	assert(ubi.nTotal >= nSize);
	if(ubi.nSuffix >= nSize)//后置空间够，直接追入
	{
		unsigned char* pTmp = m_pBuf + m_nTailPos + 1;
		memmove(pTmp, pData, nSize*sizeof(unsigned char));
		m_nData += nSize;
		m_nTailPos += nSize;
		if(m_nHeadPos == -1) m_nHeadPos=0;//第一次添加数据
	}
	else//需要数据折返存入
	{
		unsigned char* pTmp = 0;
		if(ubi.nSuffix > 0)//后置有空间，先用后置空间
		{
			pTmp = m_pBuf + m_nTailPos + 1;
			memmove(pTmp, pData, ubi.nSuffix*sizeof(unsigned char));
		}
		//再用前置空间
		assert(ubi.nPrefix >= (nSize-ubi.nSuffix));
		pTmp = pData + ubi.nSuffix;
		int nr = nSize - ubi.nSuffix;
		memmove(m_pBuf, pTmp, nr*sizeof(unsigned char));
		m_nData += nSize;
		m_nTailPos = nr - 1;
		assert(m_nTailPos < m_nHeadPos);
	}
	return true;
}
//______________________________________________________________________________________
//从缓冲区头部顺序取出数据，返回取出数据长度
int CRoundDataBuff::GetData(unsigned char* pData, int nSize)
{
	if(pData==0 || nSize<=0)	return 0;
	if(m_pBuf == 0){return 0;}//没数据
	if(m_nData == 0){return 0;}//没数据
	if(m_nTailPos==-1 && m_nHeadPos==-1){return 0;}//没数据
	
	int nGet = min(m_nData, nSize);//取的数据大小不能大于总数据大小
	int nRemain = m_nData - nGet;//会剩下多少数据
	unsigned char* pHead = m_pBuf + m_nHeadPos;//头位置指针
	if(m_nTailPos >= m_nHeadPos)//数据连续存入的
	{
		memmove(pData, pHead, nGet*sizeof(unsigned char));
		m_nData = nRemain;
		//数据全部取走了
		if(m_nData == 0)
		{
			m_nHeadPos = -1;
			m_nTailPos = -1;
		}
		//缓冲区还有数据
		else //m_nData > 0
		{
			m_nHeadPos += nGet;
			assert(m_nHeadPos <= m_nTailPos);
		}
		//清除一下数据
		//ZEROMEM(pHead, nGet*sizeof(unsigned char));
	}
	else//m_nTailPos < m_nHeadPos 数据是折返存入的
	{
		int nBackCnt = m_nSize - m_nHeadPos;
		int nCpy = min(nBackCnt, nGet);
		memmove(pData, pHead, nCpy*sizeof(unsigned char));
		//ZEROMEM(pHead, nCpy*sizeof(unsigned char));
		if(nBackCnt < nGet)//要取折返数据
		{
			//从最前面取剩下的数据
			nCpy = nGet - nBackCnt;
			memmove(pData+nBackCnt, m_pBuf, nCpy*sizeof(unsigned char));
			//ZEROMEM(m_pBuf, nCpy*sizeof(unsigned char));
		}
		m_nData = nRemain;
		//数据全部取走了
		if(m_nData == 0)
		{
			m_nHeadPos = -1;
			m_nTailPos = -1;
		}
		//缓冲区还有数据
		else //m_nData > 0
		{
			if(nBackCnt <= nGet)//头位置要折返到前面
			{
				m_nHeadPos = nGet - nBackCnt;
				assert(m_nHeadPos <= m_nTailPos);
			}
			else//nBackCnt > nGet 头还在后面
			{
				m_nHeadPos += nGet;
				assert(m_nHeadPos >= m_nTailPos);
			}
		}
	}
	return nGet;
}
//______________________________________________________________________________________
//从缓冲区头部复制出数据不会将数据取出，返回复制出数据的长度
int CRoundDataBuff::PeekData(unsigned char* pData, int nSize)
{
	if(pData==0 || nSize<=0)	return 0;
	if(m_pBuf == 0){return 0;}//没数据
	if(m_nData == 0){return 0;}//没数据
	if(m_nTailPos==-1 && m_nHeadPos==-1){return 0;}//没数据
    
	int nGet = min(m_nData, nSize);//取的数据大小不能大于总数据大小
	unsigned char* pHead = m_pBuf + m_nHeadPos;//头位置指针
	if(m_nTailPos >= m_nHeadPos)//数据连续存入的
	{
		memmove(pData, pHead, nGet*sizeof(unsigned char));
	}
	else//m_nTailPos < m_nHeadPos 数据是折返存入的
	{
		int nBackCnt = m_nSize - m_nHeadPos;
		int nCpy = min(nBackCnt, nGet);
		memmove(pData, pHead, nCpy*sizeof(unsigned char));
		if(nBackCnt < nGet)//要取折返数据
		{
			//从最前面取剩下的数据
			nCpy = nGet - nBackCnt;
			memmove(pData+nBackCnt, m_pBuf, nCpy*sizeof(unsigned char));
		}
	}
	return nGet;
}
//______________________________________________________________________________________
//清空缓冲区内所有数据
void CRoundDataBuff::Empty()
{
	if(m_pBuf != 0)
	{
		//ZEROMEM(m_pBuf, m_nSize*sizeof(unsigned char));
		m_nData = 0;
		m_nHeadPos = -1;
		m_nTailPos = -1;
	}
}
//______________________________________________________________________________________
//判断缓冲区是否为空
bool CRoundDataBuff::IsEmpty()
{
	return (m_nData==0);
}
//______________________________________________________________________________________
//释放多余的内存空间，返回实际剩余空间大小
int CRoundDataBuff::FreeExtra()
{
	if(m_pBuf == 0) return 0;
	//如果没有数据则清除所有内在空间
	if(m_nData == 0)
	{
		free(m_pBuf);
		m_pBuf = 0;
		m_nSize = 0;
		m_nHeadPos = -1;
		m_nTailPos = -1;
		return 0;
	}
	//没有空余
	if(m_nData == m_nSize) return 0;
	//根据当前缓冲区数据总大小开辟新内存
	int nGet = m_nData;
	unsigned char* pTmp = (unsigned char*)malloc(nGet*sizeof(unsigned char));
	if(pTmp == 0)
	{
		assert(0);
		return m_nSize;//新开内存失败返回目前大小
	}
	//将缓冲区内的所有数据全部取出存入新开辟的内存空间
	GetData(pTmp, nGet);
	free(m_pBuf);
	m_pBuf = pTmp;
	m_nData = nGet;
	m_nSize = nGet;
	m_nHeadPos = 0;
	m_nTailPos = m_nSize - 1;
	return m_nSize;
}
//______________________________________________________________________________________
//获得缓冲区空间大小
int CRoundDataBuff::GetBuffSize()
{
	return m_nSize;
}
//______________________________________________________________________________________
//获得缓冲区内数据大小
int CRoundDataBuff::GetCount()
{
	return m_nData;
}
//______________________________________________________________________________________
//设置缓冲区最大可分配空间返回之前值，-1为无上限，默认-1
int CRoundDataBuff::SetBuffMaxLimite(int nMax)
{
	int nOld = m_nMaxSize;
	m_nMaxSize = nMax;
	return nOld;
}
//______________________________________________________________________________________
//计算剩余空间
void CRoundDataBuff::CalcFreeBuff(UNUSEBUFFINFO& ubi)
{
	//没有数据
	if(m_nTailPos==-1 && m_nHeadPos==-1)
	{
		ubi.nPrefix = 0;
		ubi.nSuffix = m_nSize;
		ubi.nTotal = ubi.nPrefix + ubi.nSuffix;
	}
	//数据是连续存储的，等于说明就一个数据
	else if(m_nTailPos >= m_nHeadPos)
	{
		ubi.nPrefix = m_nHeadPos;
		ubi.nSuffix = m_nSize - m_nTailPos - 1;
		ubi.nTotal = ubi.nPrefix + ubi.nSuffix;
	}
	//数据折返存储的
	else if(m_nTailPos < m_nHeadPos)
	{
		ubi.nPrefix = 0;
		ubi.nSuffix = m_nHeadPos - m_nTailPos - 1;
		ubi.nTotal = ubi.nPrefix + ubi.nSuffix;
	}
}
//______________________________________________________________________________________
//开辟新空间,nSize为新增加的空间大小
bool CRoundDataBuff::ReallocBuff(int nSize)
{
	if(nSize <= 0) return false;
	//超过上限了
	if(m_nMaxSize >= 0)
	{
		if((nSize+m_nSize) > m_nMaxSize) return false;
	}
	//没有分配过内存，按当前大小分配
	if(m_pBuf == 0)
	{
		m_pBuf = (unsigned char*)malloc(nSize*sizeof(unsigned char));
		if(m_pBuf == 0)
		{
			m_nSize = 0;
			m_nData = 0;
			m_nHeadPos = -1;
			m_nTailPos = -1;
			assert(0);
			return false;
		}
		//ZEROMEM(m_pBuf, nSize*sizeof(unsigned char));
		m_nSize = nSize;
		m_nData = 0;
		m_nHeadPos = -1;
		m_nTailPos = -1;
		return true;
	}
	//重新分配内存空间
	unsigned char* pNew = (unsigned char*)realloc(m_pBuf, (m_nSize+nSize)*sizeof(unsigned char));
	if(pNew == 0){assert(0); return false;}
	m_pBuf = pNew;
	int nOldSize = m_nSize;
	m_nSize += nSize;
    
	//数据没有折返或空数据，直接在后面开辟了
	if(m_nTailPos >= m_nHeadPos)
	{
		//清空未用内存,如果尾是-1的话说明没有数据则全清零
		//unsigned char* pTmp = m_pBuf + m_nTailPos + 1;
		//ZEROMEM(pTmp, (m_nSize-(m_nTailPos+1))*sizeof(unsigned char));
	}
	//数据有折返在中间开辟新空间
	else//m_nTailPos < m_nHeadPos
	{
		//把头以后的数据向后移动到最后，将新开辟的空间插到头之前
		unsigned char* pTmp = m_pBuf + m_nHeadPos;
		unsigned char* pTo = pTmp + nSize;
		int nCnt = nOldSize - m_nHeadPos;
		memmove(pTo, pTmp, nCnt*sizeof(unsigned char));
		//清一下新插进来的内存
		//ZEROMEM(pTmp, nSize*sizeof(unsigned char));
		m_nHeadPos += nSize;
	}
	return true;
}