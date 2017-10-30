#include "thread_mgr.h"
#include "event_wrapper.h"
#include "mutex_wrapper.h"
#include "sleep.h"

CThreadMgr::CThreadMgr(void)
: mIsStop( false )
,mCS(NULL)
,mEvent(NULL)
,mThread(NULL)
{
	mCS = CriticalSectionWrapper::CreateCriticalSection();
	mEvent = EventWrapper::Create();
}

CThreadMgr::~CThreadMgr(void)
{
	if(mThread)
		delete mThread;
	if(mEvent)
		delete mEvent;
	if(mCS)
		delete mCS;
}

bool CThreadMgr::createThread(const char* name, eThreadPriority iPriority)
{
	if (mThread){
		//ReleaseThread();
		return true;
	}

	mThread = ThreadWrapper::CreateThread(ThreadProc, this, kNormalPriority, name);
	if(!mThread)
		return false;
	unsigned int thread_id;
	if(!mThread->Start(thread_id))
	{
		return false;
	}
	return true;
}

bool CThreadMgr::releaseThread()
{
	mIsStop = true;
	if (mThread){
		mThread->Stop();
		delete mThread;
		mThread = nullptr;
	}
	return true;
}

void CThreadMgr::postMessageOS( int id, void* pData )
{
	_tMsg msg = { false, id, pData };
	
	{
		CriticalSectionScoped lock(mCS);
		mLstMsg.push_back( msg );
	}
}

bool CThreadMgr::sendMessageOS( int id, void* pData )
{
	_tMsg msg = { true, id, pData };

	{
		CriticalSectionScoped lock(mCS);
		mLstMsg.push_front( msg );
	}
	

	if(mEvent->Wait(UTIL_EVENT_INFINITE) == kEventSignaled)
	{
		return true;
	}

	return false;
}

void CThreadMgr::processIdle()
{
	SleepMs(50);
}

bool CThreadMgr::ThreadProc(ThreadObj pData)
{
	CThreadMgr * pThis = static_cast<CThreadMgr *>(pData);
	return pThis->run();
}

bool CThreadMgr::run()
{
	while ( !mIsStop )
	{
		bool bIsEmpty = false;
		_tMsg msg = { false, MSG_INVALID, 0 };
		
		{
			CriticalSectionScoped lock(mCS);
			bIsEmpty = mLstMsg.empty();
			if ( !bIsEmpty )
			{
				msg = mLstMsg.front();
				mLstMsg.pop_front();
			}
		}

		
		if ( !bIsEmpty && msg.id != MSG_INVALID )//如果不为空，并且消息不为MSG_INVALID,
		{
			processMessage(msg.id,msg.data);
			if ( msg.isSend )
				mEvent->Set();
		}
		else//空闲处理
		{
			processIdle();
		}
	}

	return false;//结束线程
}
