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

#include <windows.h>

// Undefine Microsft Yield macro in order to use this as a function name
#undef Yield

//-----------------------------------------------------------------------------------------------------------
// Type of a thread smart pointer
typedef sxCTIntrusivePtr<class sxCThread>::sxCType sxCThreadPtr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Thread object implementation. At least one reference should be maintained on a created thread (using 
// a sxCThreadPtr) in order for it to stay alive. Furthermore, a thread could not be create until it has been
// joined
class sxCThread : public sxIIntrusiveCounter, private sxINonCopyable
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Destructor. The thread must have been joined before this function could be called escpet if the thread 
	// object was created from an existing os thread
	~sxCThread();

	//-------------------------------------------------------------------------------------------------------
	// Attach a sxCThread object to an existing os thread. Only one sxCThread could be attached at a time to
	// an os thread
	static sxCThreadPtr Attach(sxChar const* a_szName);

	// Define thread entry function
	typedef	void (*sxFnThreadFunction)(void*);

	//-------------------------------------------------------------------------------------------------------
	// Create a new thread that we'll execute a_pFnThreadFunction
	static sxCThreadPtr Create(	sxChar const* a_szName,
								sxFnThreadFunction a_pFnThreadFunction,
								void* a_pvArgument);

	//-------------------------------------------------------------------------------------------------------
	//.Join thread execution
	void Join();

	//-------------------------------------------------------------------------------------------------------
	// Comparison operator == 
	sxBool operator == (sxCThread const& a_rOther) const
	{
		return (m_uiThreadUUID == a_rOther.m_uiThreadUUID);
	}

	//-------------------------------------------------------------------------------------------------------
	// Comparison operator != 
	sxBool operator != (sxCThread const& a_rOther) const
	{
		return (m_uiThreadUUID != a_rOther.m_uiThreadUUID);
	}

	//-------------------------------------------------------------------------------------------------------
	// Sleep the calling thread for a_uiMilliseconds
	static void Sleep(sxUInt a_uiMilliseconds);

	//-------------------------------------------------------------------------------------------------------
	// Yield processing time to another thread
	static void Yield();

	//-------------------------------------------------------------------------------------------------------
	// Get current thread sxCThreadPtr
	static sxCThreadPtr GetCurrent();

private:

	// Constructors are private to ensure proper public function (Attach/Create) are used

	//-------------------------------------------------------------------------------------------------------
	// Construct a sxCThread to be attached to the calling thread
	sxCThread(sxChar const* a_szName);

	//-------------------------------------------------------------------------------------------------------
	// Construct a new os thread
	sxCThread(	sxChar const* a_szName,
				sxFnThreadFunction a_pFnThreadFunction,
				void* a_pvArgument);

	// _sxThreadProxy is friend in order to call operator ()
	friend sxUInt __stdcall _sxThreadProxy(void* a_pvParameter);

	// Internal operator used to start thread entry function
	void operator()();

	//-------------------------------------------------------------------------------------------------------
	// Get thread name
	sxChar const* GetName() const {return m_strName.c_str();}

	// Declares a sxCThread* in thread local storage to store calling sxCThread object
	static __declspec(thread) sxCThread* ms_plsThread;

	// Thread windows Handle
	HANDLE m_hThreadHandle;

	// Thread unique ID
	sxUInt m_uiThreadUUID;
	
	// Function to call from the proxy at thread start
	sxFnThreadFunction m_pFnThreadFunction;

	// Function argument
	void* m_pvArgument;

	// Thread name
	string m_strName;

	// Is thread joinable
	sxBool m_bJoinable;

#ifdef __sxBuildUTest
	// Has a unit test failed on this thread
	sxBool m_bUTestFailed;
#endif // __sxBuildUTest
};
