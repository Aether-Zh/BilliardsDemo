#include "ptrlist.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//_________________________________________________________________________________
//链表节点元素数据结构
typedef struct _tagNODEDATA
{
	void*			pData;		//节点数据
	_tagNODEDATA*	pNext;		//下一个节点指针
	_tagNODEDATA*	pPrev;		//上一个节点指针
	_tagNODEDATA()
	{
		pData = 0;
		pNext = 0;
		pPrev = 0;
	}
	void* operator new(size_t count)
	{
		return malloc(count);
	}
	void operator delete(void* _Ptr)
	{
		free(_Ptr);
	}
}
NODEDATA, *PNODEDATA;
//_________________________________________________________________________________
//类型转换宏，用于将PNODEDATA与NODEPOS进行互转
#define NODEPTR(x)		(reinterpret_cast<PNODEDATA>(x))	//NODEPOS 转 PNODEDATA
#define POSPTR(x)		(reinterpret_cast<NODEPOS>(x))		//PNODEDATA 转 NODEPOS
//_________________________________________________________________________________
//构造函数
CPtrlist::CPtrlist()
{
	m_pHead = 0;
	m_pTail = 0;
	m_nCount = 0;
}
//_________________________________________________________________________________
//析构函数
CPtrlist::~CPtrlist()
{
	RemoveAll();
	assert(m_pHead==0 && m_pTail==0 && m_nCount==0);
}
//_________________________________________________________________________________
//判断链表是否为空
bool CPtrlist::IsEmpty() const
{
	if(m_pHead == 0 ||
	   m_pTail == 0 ||
	   m_nCount <= 0)
        return true;
	return false;
}
//_________________________________________________________________________________
//得到链表节点总个数
int CPtrlist::GetCount() const
{
	return m_nCount;
}
//_________________________________________________________________________________
//在链表头部加入一个节点
void CPtrlist::AddHead(void* pData)
{
	PNODEDATA pNode = new NODEDATA;
	pNode->pData = pData;
	assert(pNode);
	if(IsEmpty())//第一个节点
	{
		pNode->pPrev = 0;
		pNode->pNext = 0;
		m_pHead = POSPTR(pNode);
		m_pTail = POSPTR(pNode);
	}
	else
	{
		pNode->pPrev = 0;
		pNode->pNext = NODEPTR(m_pHead);
		NODEPTR(m_pHead)->pPrev = pNode;
		m_pHead = POSPTR(pNode);
	}
	m_nCount++;
}
//_________________________________________________________________________________
//在链表尾部加入一个节点
void CPtrlist::AddTail(void* pData)
{
	PNODEDATA pNode = new NODEDATA;
	pNode->pData = pData;
	assert(pNode);
	if(IsEmpty())//第一个节点
	{
		pNode->pPrev = 0;
		pNode->pNext = 0;
		m_pHead = POSPTR(pNode);
		m_pTail = POSPTR(pNode);
	}
	else
	{
		pNode->pPrev = NODEPTR(m_pTail);
		pNode->pNext = 0;
		NODEPTR(m_pTail)->pNext = pNode;
		m_pTail = POSPTR(pNode);
	}
	m_nCount++;
}
//_________________________________________________________________________________
//得到头节点数据
void* CPtrlist::GetHead() const
{
	if(IsEmpty())
		return 0;
	return NODEPTR(m_pHead)->pData;
}
//_________________________________________________________________________________
//得到尾节点数据
void* CPtrlist::GetTail() const
{
	if(IsEmpty())
		return 0;
	return NODEPTR(m_pTail)->pData;
}
//_________________________________________________________________________________
//得到头节点位置
NODEPOS CPtrlist::GetHeadPos() const
{
	if(IsEmpty())
		return 0;
	return m_pHead;
}
//_________________________________________________________________________________
//得到尾节点位置
NODEPOS CPtrlist::GetTailPos() const
{
	if(IsEmpty())
		return 0;
	return m_pTail;
}
//_________________________________________________________________________________
//得到pos指定节点数据，并将pos修改为下一个节点的位置，
//如果pos为0，表示已经是最后一个节点
void* CPtrlist::GetNext(NODEPOS& pos)
{
	PNODEDATA pNode = NODEPTR(pos);
	if(pNode == 0)
	{
		assert(0);
		return 0;
	}
	pos = POSPTR(pNode->pNext);
	return pNode->pData;
}
//_________________________________________________________________________________
//得到pos指定节点数据，并将pos修改为上一个节点的位置，
//如果pos为0，表示已经是第一个节点
void* CPtrlist::GetPrev(NODEPOS& pos)
{
	PNODEDATA pNode = NODEPTR(pos);
	if(pNode == 0)
	{
		assert(0);
		return 0;
	}
	pos = POSPTR(pNode->pPrev);
	return pNode->pData;
}
//_________________________________________________________________________________
//删除头节点
void CPtrlist::RemoveHead()
{
	if(IsEmpty())
		return;
	if(m_pHead == m_pTail)//唯一一个节点
	{
		delete NODEPTR(m_pHead);
		m_pHead = 0;
		m_pTail = 0;
		m_nCount--;
		assert(m_nCount == 0);
		return;
	}
	//链表中有多个节点
	PNODEDATA pNode = NODEPTR(m_pHead);
	m_pHead = POSPTR(pNode->pNext);
	NODEPTR(m_pHead)->pPrev = 0;
	delete pNode;
	pNode = 0;
	m_nCount--;
	assert(m_nCount >= 1);
}
//_________________________________________________________________________________
//删除尾节点
void CPtrlist::RemoveTail()
{
	if(IsEmpty())
		return;
	if(m_pHead == m_pTail)//唯一一个节点
	{
		delete NODEPTR(m_pTail);
		m_pHead = 0;
		m_pTail = 0;
		m_nCount--;
		assert(m_nCount == 0);
		return;
	}
	//链表中有多个节点
	PNODEDATA pNode = NODEPTR(m_pTail);
	m_pTail = POSPTR(pNode->pPrev);
	NODEPTR(m_pTail)->pNext = 0;
	delete pNode;
	pNode = 0;
	m_nCount--;
	assert(m_nCount >= 1);
}
//_________________________________________________________________________________
//删除pos指定位置的节点
void CPtrlist::RemoveAt(NODEPOS pos)
{
	if(IsEmpty())
		return;
	PNODEDATA pNode = NODEPTR(pos);
	if(pNode == 0)
		return;
	if(pNode == m_pHead)//是头节点
	{
		RemoveHead();
		return;
	}
	if(pNode == m_pTail)//是尾节点
	{
		RemoveTail();
		return;
	}
	pNode->pPrev->pNext = pNode->pNext;
	pNode->pNext->pPrev = pNode->pPrev;
	delete pNode;
	pNode = 0;
	m_nCount--;
	assert(m_nCount >= 2);
}
//_________________________________________________________________________________
//删除所有节点
void CPtrlist::RemoveAll()
{
	while(!IsEmpty())
	{
		RemoveTail();
	}
	assert(m_nCount == 0);
}
//_________________________________________________________________________________
//设置pos指定节点的数据内容
void CPtrlist::SetAt(NODEPOS pos, void* pData)
{
	if(IsEmpty())
		return;
	PNODEDATA pNode = NODEPTR(pos);
	if(pNode == 0)
		return;
	pNode->pData = pData;
}
//_________________________________________________________________________________
//得到pos指定节点的数据内容
void* CPtrlist::GetAt(NODEPOS pos)
{
	if(IsEmpty())
		return 0;
	PNODEDATA pNode = NODEPTR(pos);
	if(pNode == 0)
		return 0;
	return pNode->pData;
}
//_________________________________________________________________________________
//在链表中查找数据内容与pData相同的节点的位置，
//startPos为开始查找的位置，0表示全部查找
//返回0表示没有找到
NODEPOS CPtrlist::Find(void* pData, NODEPOS startPos/* = 0*/)
{
	if(IsEmpty())
		return 0;
	NODEPOS pos = m_pHead;
	if(startPos != 0)
	{
		pos = startPos;
	}
	while(pos != 0)
	{
		PNODEDATA pNode = NODEPTR(pos);
		void* p = GetNext(pos);
		if(p == pData)
		{
			return POSPTR(pNode);
		}
	}
	return 0;
}
//_________________________________________________________________________________
//在pos指定位置的节点后插入一个节点，返回0插入失败
NODEPOS CPtrlist::InsertAfter(NODEPOS pos, void* pData)
{
	if(pos == 0)
		return 0;
	PNODEDATA pNew = new NODEDATA;
	assert(pNew != 0);
	pNew->pData = pData;
	//在最后插入
	if(pos == m_pTail)
	{
		NODEPTR(m_pTail)->pNext = pNew;
		pNew->pPrev = NODEPTR(m_pTail);
		pNew->pNext = 0;
		m_pTail = POSPTR(pNew);
		m_nCount++;
		return m_pTail;
	}
	PNODEDATA pNode = NODEPTR(pos);
	pNew->pNext = pNode->pNext;
	pNode->pNext->pPrev = pNew;
	pNew->pPrev = pNode;
	pNode->pNext = pNew;
	m_nCount++;
	return POSPTR(pNew);
}
//_________________________________________________________________________________
//在pos指定位置的节点前插入一个节点，返回0插入失败
NODEPOS CPtrlist::InsertBefore(NODEPOS pos, void* pData)
{
	if(pos == 0)
		return 0;
	PNODEDATA pNew = new NODEDATA;
	assert(pNew != 0);
	pNew->pData = pData;
	//在头部插入
	if(pos == m_pHead)
	{
		NODEPTR(m_pHead)->pPrev = pNew;
		pNew->pPrev = 0;
		pNew->pNext = NODEPTR(m_pHead);
		m_pHead = POSPTR(pNew);
		m_nCount++;
		return m_pHead;
	}
	PNODEDATA pNode = NODEPTR(pos);
	pNew->pPrev = pNode->pPrev;
	pNode->pPrev->pNext = pNew;
	pNew->pNext = pNode;
	pNode->pPrev = pNew;
	m_nCount++;
	return POSPTR(pNew);
}