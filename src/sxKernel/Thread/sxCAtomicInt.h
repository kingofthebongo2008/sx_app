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

#include <intrin.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Interlock intrinsics declaration

// To declare an interlocked function for use as an intrinsic, First, the function must be declared with the 
// leading underscore. Second, the new function must appear in a #pragma intrinsic statement. For convenience,
// the intrinsic versions of the functions may be #define'd to appear in the source code without the leading 
// underscore.
extern "C" long  __cdecl _InterlockedCompareExchange(long volatile* a_vpDest, long a_Exchange, long a_Comp);
extern "C" long  __cdecl _InterlockedExchange(long volatile* a_vpTarget, long a_Value);
extern "C" long  __cdecl _InterlockedExchangeAdd(long volatile* a_vpAddend, long a_Value);
extern "C" long  __cdecl _InterlockedIncrement(long volatile* a_vpAddend);
extern "C" long  __cdecl _InterlockedDecrement(long volatile* a_vpAddend);

// Specifies that calls to functions specified in the pragma's argument list are intrinsic. The compiler 
// generates intrinsic functions as inline code, not as function calls.
#pragma intrinsic (_InterlockedCompareExchange)
#pragma intrinsic (_InterlockedExchange)
#pragma intrinsic (_InterlockedExchangeAdd)
#pragma intrinsic (_InterlockedIncrement)
#pragma intrinsic (_InterlockedDecrement)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class sxCAtomicInt
{
public:

	sxCAtomicInt(sxInt a_iValue) :
		m_iValue(a_iValue)
	{
	}

	sxInt operator ++ (sxInt)
	{
		return _InterlockedExchangeAdd(&m_iValue, 1);
	}

	sxInt operator -- (sxInt)
	{
		return _InterlockedExchangeAdd(&m_iValue, -1);
	}

	sxInt operator ++ ()
	{
		return _InterlockedIncrement(&m_iValue);
	}

	sxInt operator -- ()
	{
		return _InterlockedDecrement(&m_iValue);
	}

	void operator = (sxInt a_iExchange)
	{
		_InterlockedExchange(&m_iValue, a_iExchange);
	}

	sxInt CompareExchange(sxInt a_iComperand, sxInt a_iExchange)
	{
		return _InterlockedCompareExchange(&m_iValue, a_iExchange, a_iComperand);
	}

	sxInt GetValue() const
	{
		return _InterlockedExchangeAdd(&m_iValue, 0);
	}

	operator sxInt() const
	{
		return GetValue();
	}

private:

	// Atomically modifed value. Declared mutable such that function that emulate atomic reads 
	// reads can be implemented using standard _InterlockedXXX functions 
	mutable long m_iValue;
};
