#ifndef __WORKQUEUE_H_2013_05_04_23_41__
#define __WORKQUEUE_H_2013_05_04_23_41__

#include <pthread.h>
#include "ptrqueue.h"
#include "cocos2d.h"

//用于工作线程与主线程传输数据的队列
//队列会加锁保证线程安全,
//当添加数据后队列会自动调用指定函数

typedef void (cocos2d::Ref::*SEL_workQHandler)(void*);

#define workQ_selector(_SELECTOR) (SEL_workQHandler)(&_SELECTOR)

//队列项定义
typedef struct _tagQUEUEITEM
{
    cocos2d::Ref                *pObj;      //回调对象(加入队列后retain，调完后release)
    SEL_workQHandler            pFunc;      //回调函数
	void                        *pParam;    //上下文参数
}
QUEUEITEM, *PQUEUEITEM;

//工作队列类
class CWorkQueue : public cocos2d::Ref
{
public:
	CWorkQueue();
	~CWorkQueue();
    
	//添加项
	bool PushItem(cocos2d::Ref *pObj, SEL_workQHandler pFunc, void *pParam);
	//清除
	void Clear();
    //执行回调
    void Execute(float delta);
private:
    //加锁
	void Lock();
    //解锁
	void Unlock();
    
private:
	CPtrQueue				m_queue;        //指针循环对列
	pthread_mutex_t			m_cs;           //锁对象
};

#endif //__WORKQUEUE_H_2013_05_04_23_41__