//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
//																											|
//								ShaderX7. Cross platform rendering thread. 2008.							|
//				Guillaume Blanc. ELB (Etranges Libellules) http://www.elb-games.com/. Lyon, France.			|
//																											|
//	This program is free software. It is distributed in the hope that it will be useful, but without any	|
//	warranty, without even the implied warranty of merchantability or fitness for a particular purpose.		|
//																											|
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
#pragma once

#include "sxCMutex.h"
#include <windows.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implement a condition object that works in conjunction with sxCTMutexLocker. Implementation details were 
// taken from boost thread library (http://www.boost.org/).
class sxCCondition : private sxINonCopyable
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxCCondition();

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	~sxCCondition();

	//-------------------------------------------------------------------------------------------------------
	// If there is a thread waiting on rThis, change that thread's state to ready. Otherwise there is no
	// effect.
	void NotifyOne();

	//-------------------------------------------------------------------------------------------------------
	// Change the state of all threads waiting on rThis to ready. If there are no waiting threads, NotifyAll
	// has no effect.
	void NotifyAll();

	//-------------------------------------------------------------------------------------------------------
	// Releases the lock on the mutex object associated with lock, blocks the current thread of execution
	// until readied by a call to NotifyOne() or NotifyAll(), and then reacquires the lock.
	template <typename t_ScopedLock>
	void Wait(t_ScopedLock& a_rtScopedLock)
	{
		// Verifies precondition
		sxAssert(a_rtScopedLock.IsLocked(), "ScopedLock should be block on entering condition");

		// Wait on mutex for condition to be signaled
		EnterWait();

		a_rtScopedLock.Unlock();
		sxValidateNoText(Wait(INFINITE) == true);
		a_rtScopedLock.Lock();
	}

	//-------------------------------------------------------------------------------------------------------
	// Releases the lock on the mutex object associated with lock, blocks the current thread of execution
	// until readied by a call to NotifyOne() or NotifyAll(), or until time a_uiMilliSeconds is reached.
	template <typename t_ScopedLock>
	sxBool TimedWait(t_ScopedLock& a_rtScopedLock, sxUInt a_uiMilliSecond)
	{
		// Verifies precondition
		sxAssert(a_rtScopedLock.IsLocked(),"ScopedLock should be block on entering condition");

		// Wait on mutex for condition to be signaled or timed out
		EnterWait();

		a_rtScopedLock.Unlock();
		sxBool bRet = Wait(a_uiMilliSecond);
		a_rtScopedLock.Lock();

		return bRet;
	}

private:

	//-------------------------------------------------------------------------------------------------------
	// Internal function called before Wait
	void EnterWait();

	//-------------------------------------------------------------------------------------------------------
	// Internal function that implements Wait
	sxBool Wait(sxUInt a_uiMilliSeconds);

	// Internal gate semaphore
	HANDLE m_hGateSemaphore;

	// Internal queue Semaphore
	HANDLE m_hQueueSemaphore;

	// Internal mutex
	sxCMutex m_oMutex;

	// Number of threads that timed out and never made it to m_hQueueSemaphore
	sxUInt m_uiGone;

	// Number of threads blocked on the condition
	sxUInt m_uiBlocked; 

	// Number of threads no longer waiting for the condition but still waiting to be removed from m_hQueueSemaphore
	sxUInt m_uiWaiting;
};
