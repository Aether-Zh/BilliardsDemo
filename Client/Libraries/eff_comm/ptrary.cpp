#include "ptrary.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MIN_ALLOC_GROWBY	(64)	//默认一次分配空间的个数，最少也不能少于这个值
//__________________________________________________________________________
//构造函数
CPtrAry::CPtrAry(void)
{
	m_nCount = 0;
	m_nAlloc = 0;
	m_par = 0;
	m_nGrowBy = MIN_ALLOC_GROWBY;
}
//__________________________________________________________________________
//析构函数
CPtrAry::~CPtrAry(void)
{
	if(m_par != 0)
	{
		free(m_par);
		m_par = 0;
	}
	m_nCount = 0;
	m_nAlloc = 0;
}
//__________________________________________________________________________
//得到数组元素的个数
int CPtrAry::GetCount() const
{
	return m_nCount;
}
//__________________________________________________________________________
//得到数组元素的个数
int CPtrAry::GetSize() const
{
	return GetCount();
}
//__________________________________________________________________________
//得到数组的最大下标值，返回-1说明数组没有内容
int CPtrAry::GetUpperBound() const
{
	if(IsEmpty())
		return -1;
	return m_nCount - 1;
}
//__________________________________________________________________________
//判断数组是否为空
bool CPtrAry::IsEmpty() const
{
	if(m_par == 0)
		return true;
	if(m_nCount <= 0)
		return true;
	return false;
}
//__________________________________________________________________________
//得到指定位置的元素
void* CPtrAry::GetAt(int nIdx) const
{
	if(m_par == 0)
		return 0;
	if(nIdx<0 || nIdx>=m_nCount)
		return 0;
    
	return (void*)*(m_par+nIdx);
}
//__________________________________________________________________________
//设置指定位置的元素
void CPtrAry::SetAt(int nIdx, void* pData)
{
	if(m_par == 0)
		return;
	if(nIdx<0 || nIdx>=m_nCount)
		return;
    
	*(m_par+nIdx) = pData;
}
//__________________________________________________________________________
//向数组中添加一个元素插在最后位置，并返回插入的实际位置
int CPtrAry::Add(void* pData)
{
	//没有开僻过内存
	if(m_par == 0)
	{
		m_par = (void**)malloc(sizeof(void*)*MIN_ALLOC_GROWBY);
		memset(m_par, 0, sizeof(void*)*MIN_ALLOC_GROWBY);
		m_nAlloc = MIN_ALLOC_GROWBY;
		//插入第一个元素
		*m_par = pData;
	}
	else//已有开僻的内存
	{
		//还有已分配的空间可用
		if(m_nCount < m_nAlloc)
		{
			*(m_par+m_nCount) = pData;
		}
		else//没有空间可用了，要重新分配内存
		{
			//重新分配内存空间
			void** pNew = (void**)realloc(m_par, (m_nAlloc+m_nGrowBy)*sizeof(void*));
			if(pNew == 0){assert(0);return -1;}
			m_par = pNew;
			m_nAlloc += m_nGrowBy;
			//清空未用内存
			void** pTmp = m_par + m_nCount;
			memset(pTmp, 0, sizeof(void*)*(m_nAlloc-m_nCount));
			//添加数据
			*(m_par+m_nCount) = pData;
		}
	}
	int nPos = m_nCount;
	m_nCount++;
	return nPos;
}
//__________________________________________________________________________
//删除指定位置的元素
void CPtrAry::RemoveAt(int nIdx)
{
	if(m_par == 0)
		return;
	if(nIdx<0 || nIdx>=m_nCount)
		return;
	//删除指定位置元素，并将后面的元素向前移动，并将空出的最后一个位置置成0
	int nCpyCnt = m_nCount - 1 - nIdx;
	if(nCpyCnt > 0)//有数据需要向前移动
	{
		void** p1 = m_par + nIdx;
		void** p2 = m_par + nIdx + 1;
		assert(nCpyCnt>0 && nCpyCnt<m_nCount);
		memmove(p1, p2, nCpyCnt*sizeof(void*));
	}
	else if(nCpyCnt == 0)//删除的是最后一个内容
	{
		assert(nIdx == m_nCount-1);
	}
	else//到这里nCpyCnt<0，但理论上是绝不可能小于0的，除非堆栈乱了
	{
		assert(0);
		return;
	}
	memset(m_par+(m_nCount-1), 0, sizeof(void*));
	m_nCount -= 1;
}
//__________________________________________________________________________
//删除所有数组元素
void CPtrAry::RemoveAll()
{
	if(m_par == 0)
		return;
	memset(m_par, 0, m_nCount*sizeof(void*));
	m_nCount = 0;
}
//__________________________________________________________________________
//设置数组使用内存的大小及当内存不够时需要再分配的增长步长值
//nNewSize：内存大小，如果小于数组元素总个数则后面内容被截断清空
//nGrowBy：如果添加的元素超出现有数组大小，一次性再开僻多少个空间，默认64个sizeof(void*)
void CPtrAry::SetSize(int nNewSize, int nGrowBy/* = -1*/)
{
	if(nNewSize < MIN_ALLOC_GROWBY)
		nNewSize = MIN_ALLOC_GROWBY;
	if(nGrowBy > 0)
		m_nGrowBy = nGrowBy;
    
	//没有分配过内存
	if(m_par == 0)
	{
		m_par = (void**)malloc(sizeof(void*)*nNewSize);
		memset(m_par, 0, sizeof(void*)*nNewSize);
		m_nAlloc = nNewSize;
		return;
	}
	
	//改变内存分配的大小
	if(nNewSize < m_nAlloc)//小于原尺寸
	{
		//重新分配内存空间
		void** pNew = (void**)realloc(m_par, nNewSize*sizeof(void*));
		if(pNew == 0){assert(0);return;}
		if(m_nCount >= nNewSize)
		{
			m_nCount = nNewSize;//截掉多的内容
		}
		else
		{
			//清空未用内存
			void** pTmp = pNew + m_nCount;
			memset(pTmp, 0, sizeof(void*)*(nNewSize-m_nCount));
		}
		m_par = pNew;
		m_nAlloc = nNewSize;
	}
	else if(nNewSize > m_nAlloc)//大于原尺寸
	{
		void** pNew = (void**)realloc(m_par, nNewSize*sizeof(void*));
		if(pNew == 0){assert(0);return;}
		//清空未用内存
		void** pTmp = pNew + m_nCount;
		memset(pTmp, 0, sizeof(void*)*(nNewSize-m_nCount));
		m_par = pNew;
		m_nAlloc = nNewSize;
	}
	else
	{
		//如果新大小与老的大小一样，就不会分配了
		assert(nNewSize == m_nAlloc);
	}
}
//__________________________________________________________________________
//释放数组使用过程中分配出来的多余备用内存，使内存大小刚好与元素个数相同
void CPtrAry::FreeExtra()
{
	if(m_par == 0)
		return;
	if(m_nAlloc <= 0)
		return;
	if(m_nCount <= 0)
	{
		free(m_par);
		m_par = 0;
		m_nCount = 0;
		m_nAlloc = 0;
		return;
	}
	if(m_nAlloc == m_nCount)
		return;
	void** pNew = (void**)realloc(m_par, m_nCount*sizeof(void*));
	if(pNew == 0){assert(0);return;}
	m_par = pNew;
	m_nAlloc = m_nCount;
}
//__________________________________________________________________________
//将对象src中的数据全部复制到本对象中
void CPtrAry::Copy(const CPtrAry& src)
{
	RemoveAll();
	if(src.IsEmpty())
		return;
	int nCnt = src.GetCount();
	for(int i=0; i<nCnt; i++)
	{
		Add(src.GetAt(i));
	}
}
//__________________________________________________________________________
//在指定位置插入元素，原位置及后面元素的位置向后移动，nCount为插入多少个pData
void CPtrAry::InsertAt(int nIdx, void* pData, int nCount/* = 1*/)
{
	if(nCount<=0 || nIdx<0)
	{
		assert(0);
		return;
	}
	//在现有数组中后接数据的情况
	if(nIdx >= GetCount())
	{
		int nAddCnt = nIdx + nCount - GetCount();//一共要插多少个数据
		for(int i=0; i<nAddCnt; i++)
		{
			if(nIdx > GetCount())//还没到该插入的位置
			{
				Add(0);//在nIdx前的内容全部为0
			}
			else
			{
				Add(pData);//将数据加入
			}
		}
		return;
	}
	//在现有数据列表中插入数据的情况
	int nTotalCnt = GetCount() + nCount;
	//空间不够需要开僻
	if(nTotalCnt > m_nAlloc)
	{
		SetSize(nTotalCnt);
	}
	assert(m_nAlloc >= nTotalCnt);
	void** pSrc = (void**)(m_par + nIdx);
	int nPos = nIdx + nCount;
	void** pDes = m_par + nPos;
	int nCpyCnt = GetCount() - nIdx;
	memmove(pDes, pSrc, nCpyCnt*sizeof(void*));
	for(int i=0; i<nCount; i++)
	{
		*(pSrc+i) = pData;
	}
	m_nCount += nCount;
}
//__________________________________________________________________________
//在指定位置将pNewAry指向的数组插入到本数组中
void CPtrAry::InsertAt(int nIdx, CPtrAry* pNewAry)
{
	if(pNewAry == 0)
	{
		assert(0);
		return;
	}
	int nCount = pNewAry->GetCount();
	if(nCount<=0 || nIdx<0)
	{
		assert(0);
		return;
	}
	//先使用此函数开僻出足够的空位置
	InsertAt(nIdx, 0, nCount);
	//将新数组中的数据写入本数组
	for(int i=0; i<nCount; i++)
	{
		SetAt(nIdx+i, pNewAry->GetAt(i));
	}
}
//__________________________________________________________________________
//将src数组中的元素追加到本数组后，返回追加第一个数据的位置
int CPtrAry::Append(const CPtrAry& src)
{
	int nIdx = m_nCount;
	for(int i=0; i<src.GetCount(); i++)
	{
		Add(src.GetAt(i));
	}
	return nIdx;
}
//__________________________________________________________________________
//得到数组原始数据块的指针，一般用于qsort(...)的快速排序等功能
void** CPtrAry::GetData( ) const
{
	return (void**)m_par;
}
//__________________________________________________________________________
//操作[]符重载，可以以传统访问数组的方式访问
void* CPtrAry::operator[](int index)
{
	return GetAt(index);
}