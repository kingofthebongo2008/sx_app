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
#include "sxCThread.h"
#include <process.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main thread registration

//-----------------------------------------------------------------------------------------------------------
//
static void RegisterMainThread(sxERegisteredOperation::EValue a_eRegisteredOp)
{
	// Main thread ptr
	static sxCThreadPtr s_spMainThread = NULL;

	if(a_eRegisteredOp == sxERegisteredOperation::eInitialize)
	{
		// Instantiate main thread object
		s_spMainThread = sxCThread::Attach("Main thread");
	}
	else
	{
		// Release main thread ptr
		s_spMainThread = NULL;
	}
}

// Register function
sxRegisterFunction(&RegisterMainThread,	sxERegisterPrimaryPriority::eReserved,
										sxERegisterSecondaryPriority::eHigh);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCThread implementation

// Declares a sxCThread pointer in thread local storage to store calling thread sxCThread object
__declspec(thread) sxCThread* sxCThread::ms_plsThread = NULL;


//-----------------------------------------------------------------------------------------------------------
// _sxCRaiiThreadNameInfo implementation.
// used to associate a name to a thread that can be visible in the debugger.
class _sxCRaiiThreadNameInfo
{
public:
	_sxCRaiiThreadNameInfo(sxChar const* a_szName) :
		m_dwType(0x1000),
		m_szName((LPCSTR)a_szName),
		m_dwThreadID(0xffffffff),
		m_dwFlags(0)
	{
		// Send name via an exception to the debugger
		__try 
		{ 
			RaiseException( 0x406D1388, 
							0, 
							sizeof(_sxCRaiiThreadNameInfo) / sizeof(DWORD), 
							(ULONG_PTR const *)this ); 
		}
		__except(EXCEPTION_CONTINUE_EXECUTION){}
	}

private:
	//! Must be 0x1000
	DWORD m_dwType;	
	//! pointer to name (in user addr space)
	LPCSTR m_szName;	
	//! thread ID (-1 = caller thread)
	DWORD m_dwThreadID;	
	//! reserved for future use, must be zero
	DWORD m_dwFlags; 
};

//-----------------------------------------------------------------------------------------------------------
//
sxCThread::sxCThread(sxChar const* a_szName) :
	sxIIntrusiveCounter(1),
	m_hThreadHandle(NULL),
	m_uiThreadUUID(0),
	m_pFnThreadFunction(NULL),
	m_pvArgument(NULL),
	m_strName(a_szName),
	m_bJoinable(false)
	sxIfBuildUTest(,m_bUTestFailed(false))
{
	// Duplicate current thread handle
	sxValidate(DuplicateHandle(	GetCurrentProcess(),
								GetCurrentThread(),
								GetCurrentProcess(),
								&m_hThreadHandle,
								SYNCHRONIZE | THREAD_QUERY_INFORMATION | THREAD_SET_INFORMATION,
								false,
								0) != FALSE,
								"Thread handle duplication fialed");
	// Get thread UUID
	m_uiThreadUUID = GetCurrentThreadId();

	// Set current thread pointer
	sxAssertNoText(sxCThread::ms_plsThread == NULL);
	ms_plsThread = this;

	// Create name raii object
	_sxCRaiiThreadNameInfo oName(GetName());
}

//-----------------------------------------------------------------------------------------------------------
//
sxUInt __stdcall _sxThreadProxy(void* a_pvParameter)
{
	sxAssertNoText(a_pvParameter != NULL);

	// Fetch thread object
	sxCThread* pThread = (sxCThread*)a_pvParameter;

	// Set current thread pointer
	sxAssertNoText(sxCThread::ms_plsThread == NULL);
	sxCThread::ms_plsThread = pThread;

	// Create name raii object
	_sxCRaiiThreadNameInfo oName(pThread->GetName());

	// Execute it
	(*pThread)();

	// Thread's dying, reset current thread pointer as nobody could get it (::GetCurrent) anymore
	sxAssertNoText(sxCThread::ms_plsThread != NULL);
	sxCThread::ms_plsThread = NULL;
	
	return 0;
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCThread::operator()()
{
#ifdef __sxBuildUTest

	// Execute the thread function inside a try/catch as any unit test failure will be reported with an 
	// exception throw
	try
	{
		// Execute user function
		(*m_pFnThreadFunction)(m_pvArgument);
	}
	catch(_sxCUTestFailureException&)
	{
		// Stores that a test has failed
		m_bUTestFailed = true;
	}

#else // __sxBuildUTest

	// Execute user function
	(*m_pFnThreadFunction)(m_pvArgument);

#endif // __sxBuildUTest
}

//-----------------------------------------------------------------------------------------------------------
//
sxCThread::sxCThread(	sxChar const* a_szName,
						sxFnThreadFunction a_pFnThreadFunction,
						void* a_pvArgument) :
	sxIIntrusiveCounter(1),
	m_hThreadHandle(NULL),
	m_uiThreadUUID(0),
	m_pFnThreadFunction(a_pFnThreadFunction),
	m_pvArgument(a_pvArgument),
	m_strName(a_szName),
	m_bJoinable(true)
	sxIfBuildUTest(,m_bUTestFailed(false))
{
	// Create a new thread in active mode, _beginthreadex must be used cos CreateThread does not initialize CRT.
	m_hThreadHandle = (HANDLE)_beginthreadex(	NULL,
												64<<10,
												_sxThreadProxy,
												this,
												0,
												&m_uiThreadUUID);

	sxAssert(m_hThreadHandle != NULL, "Thread creation fails.");
}

//-----------------------------------------------------------------------------------------------------------
//
sxCThread::~sxCThread()
{
	sxAssert(m_bJoinable == false, "A thread should be joined before being destroyed");

	// Release thread handle
	if(m_hThreadHandle != NULL)
	{
		CloseHandle(m_hThreadHandle);
		m_hThreadHandle = NULL;
	}
}

//-----------------------------------------------------------------------------------------------------------
//
sxCThreadPtr sxCThread::Attach(sxChar const* a_szName)
{
	sxAssert(sxCThread::ms_plsThread == NULL, "This thread is alredy attached to a sxCThread object");

	// Instantiate a new thread object
	sxCThreadPtr spThread = sxNew(sxCThread, a_szName);
	sxValidate(spThread->DecRef() >= 1, "Invalid ref count while creating a thread");

	// Return the new  thread
	return spThread;
}

//-----------------------------------------------------------------------------------------------------------
//
sxCThreadPtr sxCThread::Create(	sxChar const* a_szName,
								sxFnThreadFunction a_pFnThreadFunction,
								void* a_pvArgument)
{
	sxAssert(a_pFnThreadFunction != NULL, "Invalid thread function");

	// Instantiate a new thread object
	sxCThreadPtr spThread = sxNew(sxCThread, a_szName, a_pFnThreadFunction, a_pvArgument);
	sxValidate(spThread->DecRef() >= 1, "Invalid ref count while creating a thread");

	// Return the new  thread
	return spThread;
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCThread::Join()
{
	// See race condition comment below
	sxAssert(m_bJoinable, "This thread isn't joinable");

	// Just infinitly wait for thread termination
	sxValidate(	WaitForSingleObject(m_hThreadHandle, INFINITE) == WAIT_OBJECT_0,
				"WaitForSingleObject INFINITE on a thread should always return true");

	// Thread might have been joined by two threads
	sxAssert(m_bJoinable, "This thread isn't joinable");

	// Thread is not joinable any more
    // This isn't a race condition since any race that could occur would have us in undefined behavior 
	// territory any way. 
	m_bJoinable = false;

#ifdef __sxBuildUTest
	// Forward UTest failure if any
	if(m_bUTestFailed == true)
	{
		// Notify a unit test has failed on this thread
		sxUTest(false);
	}
#endif // __sxBuildUTest
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCThread::Sleep(sxUInt a_uiMilliseconds)
{
	if(a_uiMilliseconds != 0)
	{
		::Sleep(a_uiMilliseconds);
	}
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCThread::Yield()
{
	// Specify 0 to indicate that this thread should be suspended to allow other waiting threads to execute.
	::Sleep(0);
}

//-----------------------------------------------------------------------------------------------------------
//
sxCThreadPtr sxCThread::GetCurrent()
{
	// Get current thread pointer from the local storage
	sxAssert(	sxCThread::ms_plsThread != NULL,
				"Invalid function call, no sxCThread object associated to that thread");

	return ms_plsThread;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Thread unit tests
#ifdef __sxBuildUTest

//-----------------------------------------------------------------------------------------------------------
//
void UTestThreadOperators()
{
	sxCThreadPtr spThread1 = sxCThread::GetCurrent();
	sxCThreadPtr spThread2 = sxCThread::GetCurrent();

	// Test operators
	sxUTest(spThread1 == spThread2);
	sxUTest((spThread1 != spThread2) == false);
}

// Register test
sxRegisterUTestCase(UTestThreadOperators, sxERegisterSecondaryPriority::eAboveNormal, "Thread operators");

//-----------------------------------------------------------------------------------------------------------
//
void _ThreadEmptyEntry(void* a_pvArgument)
{
	sxUTest(a_pvArgument == (void*)0x01234567);
}

//-----------------------------------------------------------------------------------------------------------
//
void UTestThreadCreation()
{
	sxCThreadPtr spThread1 = sxCThread::GetCurrent();

	// Could not attach two times the same thread
	sxUTestAssert(sxCThread::Attach("UTest thread attached twice"));

	// Create a new thread
	sxCThreadPtr spThread2 = sxCThread::Create("UTest thread", _ThreadEmptyEntry, (void*)0x01234567);

	// Test an assertion on this thread
	sxUTestAssert(sxAssertNoText(false));

	// Test operators
	sxUTest(spThread1 != spThread2);

	// Join it before destruction
	spThread2->Join();
}

// Register test
sxRegisterUTestCase(UTestThreadCreation, sxERegisterSecondaryPriority::eAboveNormal, "Thread creation");

#endif // __sxBuildUTest
