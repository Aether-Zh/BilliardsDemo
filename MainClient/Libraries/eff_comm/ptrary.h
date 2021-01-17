/******************************************************************\
 动态指针数组类
 文件：	PtrAry.h
 创建：
 日期：	2010-04-30
 环境：	x86、x64
 说明：	数组操作与链表的区别在于使用下标对元素进行索引效率很快
 但对于插入和删除中间元素的操作会比链表效率低
 本动态指针数组使用预先分配内存方式提高添加元素效率
 当数组中有元素时最小会占用64*sizeof(void*)个内存空间
 \******************************************************************/
#ifndef __PTRARY_H_2010_04_30_18_04___
#define __PTRARY_H_2010_04_30_18_04___

class CPtrAry
{
public:
	//构造函数
	CPtrAry();
    
	//析构函数
	~CPtrAry();
    
    //Bounds
public:
	//得到数组元素的个数
	int GetCount() const;
    
	//得到数组元素的个数
	int GetSize() const;
    
	//得到数组的最大下标值，返回-1说明数组没有内容
	int GetUpperBound() const;
    
	//设置数组使用内存的大小及当内存不够时需要再分配的增长步长值
	//调用后只是分配内存，不会向数组添加元素
	//nNewSize：内存大小，如果小于数组元素总个数则后面内容被截断清空
	//nGrowBy：如果添加的元素超出现有数组大小，一次性再开僻多少个空间，默认64个sizeof(void*)
	void SetSize(int nNewSize, int nGrowBy = -1);
    
    //Operations
public:
	//释放数组使用过程中分配出来的多余备用内存，使内存大小刚好与元素个数相同
	void FreeExtra();
    
	//判断数组是否为空
	bool IsEmpty() const;
    
	//删除所有数组元素但不会释放内部使用的内存
	void RemoveAll();
    
    //Element Access
public:
	//得到指定位置的元素
	void* GetAt(int nIdx) const;
    
	//得到数组原始数据块的指针，一般用于qsort(...)的快速排序等功能
	void** GetData() const;
    
	//设置指定位置的元素，如果nIdx没有对应项则没有操作
	void SetAt(int nIdx, void* pData);
    
    //Growing the Array
public:
	//向数组中添加一个元素插在最后位置，并返回插入的实际位置
	int Add(void* pData);
    
	//将src数组中的元素追加到本数组后，返回追加第一个数据的位置
	int Append(const CPtrAry& src);
    
	//将对象src中的数据全部复制到本对象中
	void Copy(const CPtrAry& src);
    
    //Insertion/Removal
public:
	//在指定位置插入元素，原位置及后面元素的位置向后移动，nCount为插入多少个pData
	void InsertAt(int nIdx, void* pData, int nCount = 1);
	
	//在指定位置将pNewAry指向的数组插入到本数组中
	void InsertAt(int nIdx, CPtrAry* pNewAry);
    
	//删除指定位置的元素
	void RemoveAt(int nIdx);
    
    //Operators
public:
	//操作[]符重载，可以以传统访问数组的方式访问
	void* operator[](int index);
    
protected:
	int		m_nCount;	//数组元素个数
	int		m_nAlloc;	//分配了多少内存空间
	int		m_nGrowBy;	//一次分配多少个内存空间（每个空间一个sizeof(void*)大小）
	void**	m_par;		//数组内部内存指针
};

#endif //__PTRARY_H_2010_04_30_18_04___