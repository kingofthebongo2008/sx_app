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

#include <assert.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Static assertion. Generate a compile time assertion using a compiler error report
template<sxBool x_bCondidition, typename x_AssertMessage>	struct _sxSTStaticAssertionFailure;
template<typename x_AssertMessage> struct _sxSTStaticAssertionFailure<true, x_AssertMessage> { enum { eValue = 1 }; };
template<sxSizeT x_stUnused> struct _sxSTStaticAssertionTest{};

#define sxStaticAssert(x_bExpression, x_Message) \
	class _##x_Message; \
		typedef _sxSTStaticAssertionTest<sizeof(_sxSTStaticAssertionFailure<(sxBool)(x_bExpression), _##x_Message>)> \
		sxSTStaticAssertionTestTypedef##__COUNTER__

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Run time assertion. Assertion condition is removed (not compiled) when __sxBuildAssert isn't defined.

#ifdef __sxBuildAssert
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enabled assertions implementation
 
//-----------------------------------------------------------------------------------------------------------
// Assertion maco definition
#define sxAssert(x_bCond, ...)\
{\
	if((x_bCond) == false)\
	{\
		if(sxNsAssertion::sxAssertionFailureReport(__VA_ARGS__))\
		{\
			__debugbreak();\
		}\
	}\
}

//-----------------------------------------------------------------------------------------------------------
// Internal function displaying assertion message
namespace sxNsAssertion
{
	//-------------------------------------------------------------------------------------------------------
	// Asssertion message box display. Return true if the user chooses to break execution
	sxBool sxAssertionFailureReport(sxChar const* szFormat, ...);

#ifdef __sxBuildUTest

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Raii object taht enable UTest assertion report rather that default report
	class sxCUTestAssertionEnabler
	{
	public:

		//---------------------------------------------------------------------------------------------------
		// Constructor. Enable UTest report
		sxCUTestAssertionEnabler()
		{
			ms_blsEnabled = true;
		}

		//---------------------------------------------------------------------------------------------------
		// Constructor. Disable UTest report
		~sxCUTestAssertionEnabler()
		{
			ms_blsEnabled = false;
		}

		//---------------------------------------------------------------------------------------------------
		// Test if UTest report is enabled
		static sxBool IsEnabled()
		{
			return ms_blsEnabled;
		}

	private:

		// Declares a boolean value in thread local storage
		static __declspec(thread) sxBool ms_blsEnabled;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Unit test assertion report exception
	class sxCUTestAssertionException : public exception {};

#endif // __sxBuildUTest
}

//-----------------------------------------------------------------------------------------------------------
// Helper functions
#define sxIfBuildAssertion(...) __VA_ARGS__
#define sxIfNotBuildAssertion(...) __noop

#else // __sxBuildAssert

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Disabled assertions implementation

//-----------------------------------------------------------------------------------------------------------
// Assertion maco replacement definition
#define sxAssert(x_bCond, ...)

//-----------------------------------------------------------------------------------------------------------
// Helper functions
#define sxIfBuildAssertion(...)
#define sxIfNotBuildAssertion(...) __VA_ARGS__

#endif // __sxBuildAssert

//-----------------------------------------------------------------------------------------------------------
// Validate condition. Unlike assertions, x_bCond execution is maintained for all build modes 
#define sxValidate(x_bCond, ...)\
{\
	sxIfBuildAssertion(sxBool bCond =) (x_bCond);\
	sxAssert(bCond, __VA_ARGS__);\
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Generic assertions implementation

//-----------------------------------------------------------------------------------------------------------
// Helper functions
#define sxAssertNoText(x_bCond)	sxAssert(x_bCond, "Assertion failed")
#define sxAssertUnreachableCode() sxAssert(false, "Code section should not be reached")
#define sxValidateNoText(x_bCond) sxValidate(x_bCond, "Validation failed")
