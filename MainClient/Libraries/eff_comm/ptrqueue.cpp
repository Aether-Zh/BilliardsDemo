#include "ptrqueue.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#define DEFAULT_ALLOC_GROWBY		(64)		//默认内存增长步长
//_______________________________________________________________________________________________
CPtrQueue::CPtrQueue()
{
	m_ppQueue = 0;
	m_ppHead = 0;
	m_ppTail = 0;
	m_nCount = 0;
	m_nAlloc = 0;
	m_nGrowBy = DEFAULT_ALLOC_GROWBY;
}
//_______________________________________________________________________________________________
CPtrQueue::~CPtrQueue()
{
	if(m_ppQueue != 0)
	{
		free(m_ppQueue);
		m_ppQueue = 0;
	}
	m_ppHead = 0;
	m_ppTail = 0;
	m_nCount = 0;
	m_nAlloc = 0;
}
//_______________________________________________________________________________________________
//从队列头添加元素
void CPtrQueue::PushHead(void* pData)
{
	if(m_ppQueue == 0)//没有开辟过内存
	{
		m_ppQueue = (void**)malloc(sizeof(void*)*m_nGrowBy);
		if(m_ppQueue == 0)
		{
			assert(0);
			return;
		}
		m_nAlloc = m_nGrowBy;
		//插入第一个元素
		*m_ppQueue = pData;
		m_ppHead = m_ppQueue;
		m_ppTail = m_ppQueue;
	}
	else//已开辟过内存
	{
		if(m_nCount < m_nAlloc)//有空间可用
		{
			if(m_nCount > 0)
			{
				void** ppPrev = GetPrevPos(m_ppHead);
				assert(ppPrev!=m_ppTail && ppPrev!=0);
				*ppPrev = pData;
				m_ppHead = ppPrev;
			}
			else//如果队列中没有元素，则头尾指向一个位置在此添加元素即可
			{
				*m_ppHead = pData;
			}
		}
		else if(m_nCount == m_nAlloc)//需要扩大空间
		{
			//留下头尾节点与空间头的偏移，用于重新分配内存后恢复位置
			int nHoffset = (int)(m_ppHead - m_ppQueue);
			int nToffset = (int)(m_ppTail - m_ppQueue);
			//重新分配内存空间
			void** ppNew = (void**)realloc(m_ppQueue, (m_nAlloc+m_nGrowBy)*sizeof(void*));
			if(ppNew == 0)
			{
				assert(0);
				return;
			}
			m_ppQueue = ppNew;
			m_nAlloc += m_nGrowBy;
			m_ppHead = m_ppQueue + nHoffset;
			m_ppTail = m_ppQueue + nToffset;
			if(m_ppHead == m_ppQueue)//头节点正好在空间头位置
			{
				//添加数据
				void** ppPrev = GetPrevPos(m_ppHead);
				assert(ppPrev!=m_ppTail && ppPrev!=0);
				*ppPrev = pData;
				m_ppHead = ppPrev;
			}
			else
			{
				//将头节点及之后的数据向后移动m_nGrowBy个
				int nCnt = m_nCount - (int)(m_ppHead - m_ppQueue);
				void** ppTmp = m_ppHead + m_nGrowBy;
				memmove(ppTmp, m_ppHead, nCnt*sizeof(void*));
				m_ppHead = ppTmp;
				//添加数据
				void** ppPrev = GetPrevPos(m_ppHead);
				assert(ppPrev!=m_ppTail && ppPrev!=0);
				*ppPrev = pData;
				m_ppHead = ppPrev;
			}
		}
		else //m_nCount > m_nAlloc的情况
		{
			assert(0);//如果到这里那肯定出问题了
			return;
		}
	}
	m_nCount++;
}
//_______________________________________________________________________________________________
//从队列尾添加元素
void CPtrQueue::PushTail(void* pData)
{
	if(m_ppQueue == 0)//没有开辟过内存
	{
		m_ppQueue = (void**)malloc(sizeof(void*)*m_nGrowBy);
		if(m_ppQueue == 0)
		{
			assert(0);
			return;
		}
		m_nAlloc = m_nGrowBy;
		//插入第一个元素
		*m_ppQueue = pData;
		m_ppHead = m_ppQueue;
		m_ppTail = m_ppQueue;
	}
	else//已开辟过内存
	{
		if(m_nCount < m_nAlloc)//有空间可用
		{
			if(m_nCount > 0)
			{
				void** ppNext = GetNextPos(m_ppTail);
				assert(ppNext!=m_ppHead && ppNext!=0);
				*ppNext = pData;
				m_ppTail = ppNext;
			}
			else//如果队列中没有元素，则头尾指向一个位置在此添加元素即可
			{
				*m_ppTail = pData;
			}
		}
		else if(m_nCount == m_nAlloc)//需要扩大空间
		{
			//留下头尾节点与空间头的偏移，用于重新分配内存后恢复位置
			int nHoffset = (int)(m_ppHead - m_ppQueue);
			int nToffset = (int)(m_ppTail - m_ppQueue);
			int nEoffset = m_nAlloc - 1;//老空间最后一个空间位置偏移
			//重新分配内存空间
			void** ppNew = (void**)realloc(m_ppQueue, (m_nAlloc+m_nGrowBy)*sizeof(void*));
			if(ppNew == 0)
			{
				assert(0);
				return;
			}
			m_ppQueue = ppNew;
			m_nAlloc += m_nGrowBy;
			m_ppHead = m_ppQueue + nHoffset;
			m_ppTail = m_ppQueue + nToffset;
			void** ppQend = m_ppQueue + nEoffset;
			if(m_ppTail == ppQend)//尾节点正好在空间尾位置
			{
				//添加数据
				void** ppNext = GetNextPos(m_ppTail);
				assert(ppNext!=m_ppHead && ppNext!=0);
				*ppNext = pData;
				m_ppTail = ppNext;
			}
			else
			{
				//由于元素个数与空间大小相等因此可以说明头尾节点前后相接了
				//也就是说尾节点的后面就是头节点，因此移动头节点后的数据就可以了
				int nCnt = m_nCount - (int)(m_ppHead - m_ppQueue);
				void** ppTmp = m_ppHead + m_nGrowBy;
				memmove(ppTmp, m_ppHead, nCnt*sizeof(void*));
				m_ppHead = ppTmp;
				//添加数据
				void** ppNext = GetNextPos(m_ppTail);
				assert(ppNext!=m_ppHead && ppNext!=0);
				*ppNext = pData;
				m_ppTail = ppNext;
			}
		}
		else //m_nCount > m_nAlloc的情况
		{
			assert(0);//如果到这里那肯定出问题了
			return;
		}
	}
	m_nCount++;
}
//_______________________________________________________________________________________________
//从队列头得到元素并删除队列头节点
void* CPtrQueue::PopHead()
{
	if(m_ppHead==0 || m_ppQueue==0 || m_nCount<=0)
		return 0;
    
	void* pData = *m_ppHead;
	*m_ppHead = 0;
	//如果就省一个元素了或只开辟了一个内存空间则不再移动头节点
	if(m_ppHead != m_ppTail)
	{
		void** ppNext = GetNextPos(m_ppHead);
		assert(ppNext != 0);
		m_ppHead = ppNext;
	}
	m_nCount--;
	return pData;
}
//_______________________________________________________________________________________________
//从队列尾得到元素并删除队列尾节点
void* CPtrQueue::PopTail()
{
	if(m_ppTail==0 || m_ppQueue==0 || m_nCount<=0)
		return 0;
    
	void* pData = *m_ppTail;
	*m_ppTail = 0;
	//如果就省一个元素了或只开辟了一个内存空间则不再移动头节点
	if(m_ppHead != m_ppTail)
	{
		void** ppPrev = GetPrevPos(m_ppTail);
		assert(ppPrev != 0);
		m_ppTail = ppPrev;
	}
	m_nCount--;
	return pData;
}
//_______________________________________________________________________________________________
//得到队列头节点元素
void* CPtrQueue::GetHead() const
{
	if(m_ppHead==0 || m_ppQueue==0 || m_nCount<=0)
		return 0;
	return *m_ppHead;
}
//_______________________________________________________________________________________________
//得到队列尾节点元素
void* CPtrQueue::GetTail() const
{
	if(m_ppTail==0 || m_ppQueue==0 || m_nCount<=0)
		return 0;
	return *m_ppTail;
}
//_______________________________________________________________________________________________
//得到队列总节点个数
int CPtrQueue::GetCount() const
{
	return m_nCount;
}
//_______________________________________________________________________________________________
//为队列设置当内存不足需要再分配时的增长步长
//nGrowBy：当内存不够用时一次性开辟多少内存空间
//取值增大会提高效率，但过大会浪费空间
void CPtrQueue::SetGrowBy(int nGrowBy)
{
	if(nGrowBy <= 1)
	{
		m_nGrowBy = 1;
	}
	else
	{
		m_nGrowBy = nGrowBy;
	}
}
//_______________________________________________________________________________________________
//删除队列中所用元素，但不释放内存空间
void CPtrQueue::RemoveAll()
{
	if(m_ppQueue == 0)
		return;
	m_nCount = 0;
	m_ppHead = m_ppQueue;
	m_ppTail = m_ppQueue;
}
//_______________________________________________________________________________________________
//释放多余的内容，使用队列占用的内存空间与元素个数一致
void CPtrQueue::FreeExtra()
{
	if(m_ppQueue==0 || m_nAlloc<=0)
		return;
	//没有元素，释放全部内存空间
	if(m_nCount <= 0)
	{
		free(m_ppQueue);
		m_ppQueue = 0;
		m_ppHead = 0;
		m_ppTail = 0;
		m_nCount = 0;
		m_nAlloc = 0;
		return;
	}
	//元素个数与空间相等不处理
	if(m_nAlloc == m_nCount)
		return;
	//开辟一块可以保存所有元素的新的内存空间
	void** ppTmp = (void**)malloc(sizeof(void*)*m_nCount);
	if(ppTmp == 0)
	{
		assert(0);
		return;
	}
	//将原有数据复制到新的内存空间当中
	void** ppSrc = ppTmp;
	void** ppDst = m_ppHead;
	for(int i=0; i<m_nCount; i++)
	{
		*ppSrc = *ppDst;
		ppSrc++;
		ppDst = GetNextPos(ppDst);
	}
	free(m_ppQueue);
	m_ppQueue = ppTmp;//指向新的内存空间
	m_ppHead = m_ppQueue;//空间每一个位置为头节点
	m_ppTail = m_ppQueue + m_nCount - 1;//空间最后一个位置为尾节点
	m_nAlloc = m_nCount;
}
//_______________________________________________________________________________________________
//判断当前队列是否为空
bool CPtrQueue::IsEmpty() const
{
	if(m_ppQueue==0 || m_nCount<=0)
		return true;
	return false;
}
//_______________________________________________________________________________________________
//得到当前节点下一个节点的内存位置
void** CPtrQueue::GetNextPos(void** ppPos)
{
	if(ppPos==0 || m_ppQueue==0 || m_nAlloc<=0)
		return 0;
    
	void** ppNext = 0;
	void** ppQend = m_ppQueue + m_nAlloc - 1;//空间最后一个位置
	//一定要在开辟空间的范围之内
	if(ppPos>=m_ppQueue && ppPos<=ppQend)
	{
		if(ppPos == ppQend)//到达空间尾部，转到空间第一个位置
		{
			ppNext = m_ppQueue;
		}
		else
		{
			ppNext = ppPos + 1;
		}
	}
	return ppNext;
}
//_______________________________________________________________________________________________
//得到当前节点上一个节点的内存位置
void** CPtrQueue::GetPrevPos(void** ppPos)
{
	if(ppPos==0 || m_ppQueue==0 || m_nAlloc<=0)
		return 0;
    
	void** ppPrev = 0;
	void** ppQend = m_ppQueue + m_nAlloc - 1;//空间最后一个位置
	//一定要在开辟空间的范围之内
	if(ppPos>=m_ppQueue && ppPos<=ppQend)
	{
		if(ppPos == m_ppQueue)//到达空间头部，转到空间最后一个位置
		{
			ppPrev = ppQend;
		}
		else
		{
			ppPrev = ppPos - 1;
		}
	}
	return ppPrev;
}
//_______________________________________________________________________________________________
//从头节点逐项遍历到尾节点，每项结果由回调函数返回同时返回pContext
void CPtrQueue::EnumpPtrQueue(PENUMPTRQUEUECALLBACK fpCallBack, void* pContext)
{
	if(m_ppHead == 0 ||
       m_ppTail == 0 ||
       m_ppQueue==0 ||
       m_nCount<=0 ||
       fpCallBack==0)
	{
		return;
	}
	void** pp = m_ppHead;
	if(fpCallBack(*pp, pContext) == false)
		return;
	while(pp != m_ppTail)
	{
		pp = GetNextPos(pp);
		if(fpCallBack(*pp, pContext) == false)
			return;
	}
}