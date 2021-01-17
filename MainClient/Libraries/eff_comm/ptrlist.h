/******************************************************************\
 双向动态指针链表类
 文件：	Ptrlist.h
 创建：
 日期：	2010-05-06
 环境：	x86、x64
 说明：	纯链表方式实现，每个节点有其上下节点关系
 可以很方便的在任何位置进行插入删除操作
 但对连续数据的浏览效率比动态数据要低
 \******************************************************************/
#ifndef __PTRLIST_H_2010_05_06_18_04___
#define __PTRLIST_H_2010_05_06_18_04___

//节点位置类型定义
typedef void* NODEPOS;

class CPtrlist
{
    
public:
	//构造函数
	CPtrlist();
    
	//析构函数
	~CPtrlist();
    
public:
	//在链表头部加入一个节点
	void AddHead(void* pData);
    
	//在链表尾部加入一个节点
	void AddTail(void* pData);
    
	//得到头节点数据
	void* GetHead() const;
    
	//得到尾节点数据
	void* GetTail() const;
	
	//得到头节点位置
	NODEPOS GetHeadPos() const;
    
	//得到尾节点位置
	NODEPOS GetTailPos() const;
    
	//得到pos指定节点数据，并将pos修改为下一个节点的位置，
	//如果pos为0，表示已经是最后一个节点
	void* GetNext(NODEPOS& pos);
    
	//得到pos指定节点数据，并将pos修改为上一个节点的位置，
	//如果pos为0，表示已经是第一个节点
	void* GetPrev(NODEPOS& pos);
    
	//删除头节点
	void RemoveHead();
	
	//删除尾节点
	void RemoveTail();
    
	//删除pos指定位置的节点
	void RemoveAt(NODEPOS pos);
	
	//删除所有节点
	void RemoveAll();
	
	//设置pos指定节点的数据内容
	void SetAt(NODEPOS pos, void* pData);
	
	//得到pos指定节点的数据内容
	void* GetAt(NODEPOS pos);
    
	//在链表中查找数据内容与pData相同的节点的位置，
	//startPos为开始查找的位置，0表示全部查找
	//返回0表示没有找到
	NODEPOS Find(void* pData, NODEPOS startPos = 0);
    
	//在pos指定位置的节点后插入一个节点，返回0插入失败
	NODEPOS InsertAfter(NODEPOS pos, void* pData);
    
	//在pos指定位置的节点前插入一个节点，返回0插入失败
	NODEPOS InsertBefore(NODEPOS pos, void* pData);
    
	//得到链表节点总个数
	int GetCount() const;
	
	//判断链表是否为空
	bool IsEmpty() const;
    
private:
	NODEPOS		m_pHead;		//链表头节点
	NODEPOS		m_pTail;		//链表尾节点
	int			m_nCount;		//链表节点个数
};

#endif //__PTRLIST_H_2010_05_06_18_04___