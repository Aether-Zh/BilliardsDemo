#include "workqueue.h"

USING_NS_CC;

//_____________________________________________________________________________________________________________
CWorkQueue::CWorkQueue()
{
	pthread_mutex_init(&m_cs, nullptr);
    Director::getInstance()->getScheduler()->schedule(schedule_selector(CWorkQueue::Execute), this, 0, true);
}
//_____________________________________________________________________________________________________________
CWorkQueue::~CWorkQueue()
{
	Clear();
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(CWorkQueue::Execute), this);
	pthread_mutex_destroy(&m_cs);
}
//_____________________________________________________________________________________________________________
bool CWorkQueue::PushItem(Ref* pObj, SEL_workQHandler pFunc, void* pParam)
{
	if(!pObj || !pFunc)
    {
        return false;
    }
	PQUEUEITEM pItem = new QUEUEITEM();
	if(!pItem)
    {
        return false;
    }
	pObj->retain();
	pItem->pObj = pObj;
	pItem->pFunc = pFunc;
	pItem->pParam = pParam;
	Lock();
	m_queue.PushTail((void*)pItem);
	Unlock();
    Director::getInstance()->getScheduler()->resumeTarget(this);
	return true;
}
//_____________________________________________________________________________________________________________
void CWorkQueue::Clear()
{
	Lock();
	while(!m_queue.IsEmpty())
	{
		PQUEUEITEM pItem = (PQUEUEITEM)m_queue.PopHead();
		if(pItem)
		{
            pItem->pObj->release();
			delete pItem;
		}
	}
	Unlock();
}
//_____________________________________________________________________________________________________________
void CWorkQueue::Execute(float delta)
{
	if(m_queue.IsEmpty())
	{
        Director::getInstance()->getScheduler()->pauseTarget(this);
		return;
	}
    Lock();
	PQUEUEITEM pItem = (PQUEUEITEM)m_queue.PopHead();
	Unlock();
	if(pItem)
	{
		(pItem->pObj->*pItem->pFunc)(pItem->pParam);
        pItem->pObj->release();
		delete pItem;
		pItem = 0;
	}
}
//_____________________________________________________________________________________________________________
void CWorkQueue::Lock()
{
	pthread_mutex_lock(&m_cs);
}
//_____________________________________________________________________________________________________________
void CWorkQueue::Unlock()
{
	pthread_mutex_unlock(&m_cs);
}