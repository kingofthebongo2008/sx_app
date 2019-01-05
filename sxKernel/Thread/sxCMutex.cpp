//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
//																											|
//								ShaderX7. Cross platform rendering thread. 2008.							|
//				Guillaume Blanc. ELB (Etranges Libellules) http://www.elb-games.com/. Lyon, France.			|
//																											|
//	This program is free software. It is distributed in the hope that it will be useful, but without any	|
//	warranty, without even the implied warranty of merchantability or fitness for a particular purpose.		|
//																											|
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
#include "sxKernel/sxPcHeader.h"
#include "sxCMutex.h"
#include "sxCThread.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCMutex implementation

//-----------------------------------------------------------------------------------------------------------
// 
sxCMutex::sxCMutex()
	sxIfBuildAssertion(: m_bLocked(false))
{
	InitializeCriticalSection(&m_oCriticalSection);
}

//-----------------------------------------------------------------------------------------------------------
// 
sxCMutex::~sxCMutex()
{
	// No race condition here cos we're in the destructor anyway
	sxAssert(m_bLocked == false, "Mutex should already be unlocked at destruction time");

	DeleteCriticalSection(&m_oCriticalSection);
}

//-----------------------------------------------------------------------------------------------------------
// 
void sxCMutex::Lock()
{
	EnterCriticalSection(&m_oCriticalSection);

	// m_bLocked can now be accessed as we're in the mutex protected scope

	sxAssert(m_bLocked == false, "Mutex should be unlocked at destruction time");
	sxIfBuildAssertion(m_bLocked = true);
}

//-----------------------------------------------------------------------------------------------------------
// 
sxBool sxCMutex::TryLock()
{
	if(TryEnterCriticalSection(&m_oCriticalSection) != 0)
	{
		// m_bLocked can now be accessed as we're in the mutex protected scope

		sxAssert(m_bLocked == false, "Mutex should be unlocked at destruction time");
		sxIfBuildAssertion(m_bLocked = true);

		// Try lock succeeded
		return true;
	}

	// Try lock failed
	return false;
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCMutex::Unlock()
{
	// m_bLocked can now be accessed as we're should be in the mutex protected scope
	sxAssert(m_bLocked == true, "Unlocking a mutex that isn't locked");
	sxIfBuildAssertion(m_bLocked = false);

	LeaveCriticalSection(&m_oCriticalSection);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mutex unit tests
#ifdef __sxBuildUTest

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Helper class that increment a shared value inside a mutex scope
class _sxCUTestMutexTest
{
public:
	_sxCUTestMutexTest() :
		m_uiSharedValue(0)
	{
	}

	sxBool Increment()
	{
		sxCMutexTryLocker oTryLock(m_oMutex);
		if(oTryLock.IsLocked())
		{
			return (++m_uiSharedValue == sxuiTestValue);
		}
		return false;
	}

	sxBool Test()
	{
		sxCMutexLocker oLock(m_oMutex);
		return (m_uiSharedValue == sxuiTestValue);
	}

private:

	// Value to reach
	static const sxUInt sxuiTestValue = 10000;

	sxCMutex m_oMutex;
	sxUInt m_uiSharedValue;
};

//-----------------------------------------------------------------------------------------------------------
//
void _UTestMutexThreadFunc(void* a_pvArgument)
{
	_sxCUTestMutexTest& rMutexTest = *(_sxCUTestMutexTest*)a_pvArgument;

	// Increment the shared value
	while(rMutexTest.Increment() != true)
	{
		sxCThread::Yield();
	}

	sxUTest(rMutexTest.Test());
}

//-----------------------------------------------------------------------------------------------------------
//
void UTestMutex()
{
	// Test basic locks
	{
		sxCMutex oMutex;
		sxCMutexLocker oLock(oMutex);
	}

	// Test basic try locks
	{
		sxCMutex oMutex;
		sxCMutexTryLocker oLock(oMutex);
	}

	// Test recursive locks
	{
		sxCMutex oMutex;

		sxCMutexLocker oLock(oMutex);

		// Recursive lock isn't allowed
		sxUTestAssert(sxCMutexLocker oLock(oMutex));
	}

	// Test recursive try locks
	{
		sxCMutex oMutex;

		sxCMutexLocker oLock(oMutex);

		// Recursive lock isn't allowed
		sxUTestAssert(sxCMutexTryLocker oLock(oMutex));
	}

	// Test threaded exclusion
	{
		_sxCUTestMutexTest oMutexTest;

		sxCThreadPtr spThread = sxCThread::Create("UTest mutex", _UTestMutexThreadFunc, &oMutexTest);

		// Wait while the other thread is incremented the shared value
		while(oMutexTest.Test() != true)
		{
			sxCThread::Yield();
		}

		sxUTest(oMutexTest.Test());

		spThread->Join();
	}
}

// Register test
sxRegisterUTestCase(UTestMutex, sxERegisterSecondaryPriority::eAboveNormal, "Mutex");

#endif // __sxBuildUTest
