/******************************************************************\
 双端循环队列类
 文件：	PtrQueue.h
 创建：
 日期：	2013-05-05
 环境：	x86、x64
 说明：	双端队列允许在队头或队尾进行数据添加，并可以在队头或队尾
 获得数据元素及删除头尾节点，此队列在内部使用一块循环的内存
 空间对队列中元素进行管理，当内存空间够用时不会对添加删除
 元素操作再重新进行内存分配或释放，而只是在空间不够时一次性
 成块的开辟新的内存空间，非常适用于高频率的进出队操作。
 队列不支持对中间位置元素进行操作，只支持头尾节点操作。
 \******************************************************************/
#ifndef __PTRQUEUE_H_2013_05_05_00_05__
#define __PTRQUEUE_H_2013_05_05_00_05__

//遍历队列所有元素时的回调函数类型定义
//pData:	元素项
//pContext:	上下文
//返回false终止遍历
typedef bool (*PENUMPTRQUEUECALLBACK)(void* pData, void* pContext);

class CPtrQueue
{
public:
	CPtrQueue();
	~CPtrQueue();
    
public:
	//从队列头添加元素
	void PushHead(void* pData);
    
	//从队列尾添加元素
	void PushTail(void* pData);
    
	//从队列头得到元素并删除队列头节点
	void* PopHead();
    
	//从队列尾得到元素并删除队列尾节点
	void* PopTail();
    
	//得到队列头节点元素
	void* GetHead() const;
	
	//得到队列尾节点元素
	void* GetTail() const;
    
	//得到队列总节点个数
	int GetCount() const;
    
	//为队列设置当内存不足需要再分配时的增长步长
	//nGrowBy：当内存不够用时一次性开辟多少内存空间
	//取值增大会提高效率，但过大会浪费空间
	void SetGrowBy(int nGrowBy);
    
	//删除队列中所用元素，但不释放内存空间
	void RemoveAll();
    
	//释放多余的内容，使用队列占用的内存空间与元素个数一致
	void FreeExtra();
    
	//判断当前队列是否为空
	bool IsEmpty() const;
    
	//从头节点逐项遍历到尾节点，每项结果由回调函数返回同时返回pContext
	void EnumpPtrQueue(PENUMPTRQUEUECALLBACK fpCallBack, void* pContext);
    
private:
	//得到当前节点下一个节点的内存位置
	void** GetNextPos(void** ppPos);
	
	//得到当前节点上一个节点的内存位置
	void** GetPrevPos(void** ppPos);
    
private:
	void**	m_ppQueue;		//内部内存指针
	void**	m_ppHead;		//头节点指针
	void**	m_ppTail;		//尾节点指针
	int		m_nCount;		//队列节点总数
	int		m_nAlloc;		//已分配内存空间大小
	int		m_nGrowBy;		//空间不足时新开僻内存大小
};

#endif //__PTRQUEUE_H_2013_05_05_00_05__