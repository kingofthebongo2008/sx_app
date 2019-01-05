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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Unwanted warnings
#pragma warning (disable:4275)	// Non dll-interface class used as base for dll-interface class
#pragma warning (disable:4127)	// Conditional expression is constant
#pragma warning (disable:4100)	// Unreferenced formal parameter

#include <Windows.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Crt includes
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Stl includes
#include <vector>
#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include <functional>

// Implicitly use std namespace
using namespace std;
using namespace std::placeholders;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sx preprocessor definition
#ifdef _DEBUG
#	define __sxBuildDebug
#	define __sxBuildAssert
#	define __sxBuildLog
#else // _DEBUG
#	define __sxBuildLog
#endif // _DEBUG

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sx includes (first part)
#include "sxType.h"
#include "sxMemory.h"
#include "sxAssertion.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Helper macro to get this as a reference
#define rThis (*this)

//-----------------------------------------------------------------------------------------------------------
// Asserted pointer dereferencing
template<typename t_Object>
t_Object& sxDereference(t_Object* a_ptObject)
{
	sxAssert(a_ptObject != NULL, "Dereferencing a NULL pointer");
	return *a_ptObject;
}

//-----------------------------------------------------------------------------------------------------------
// Return a reference to an invalid object. Used for compilation purpose only in unreachable code parts.
// Systematically assert when executed.
template <typename t_Object>
inline t_Object& sxGetInvalidReference()
{
	return sxDereference<t_Object>(NULL);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Helper macro to get the element count of an array
#define sxArraySize(x_aArray) (sizeof(x_aArray) / sizeof(x_aArray[0]))

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Helper macro to concat preprocessor strings
#define _sxConcat(x, y) x##y
#define sxConcat(x, y) _sxConcat(x, y)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Non copyable object interface implementation.
class sxINonCopyable
{
protected:
	//-----------------------------------------------------------------------------------------------------------
	// Constructor.
	sxINonCopyable() {}

	//-----------------------------------------------------------------------------------------------------------
	// Destructor.
	~sxINonCopyable() {}

private:

	//-----------------------------------------------------------------------------------------------------------
	// Unimplemented private copy constructor. Will cause a compiler error if used.
	sxINonCopyable(sxINonCopyable const&);

	//-----------------------------------------------------------------------------------------------------------
	// Unimplemented private copy operator. Will cause a compiler error if used.
	void operator = (sxINonCopyable const&);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Helper macro to abstract collection iteration

// for-like iteration
#define sxForEach(x_iterator, x_collectionObject, x_collectionType)\
	for(x_collectionType::iterator x_iterator = x_collectionObject.begin();\
		x_iterator != x_collectionObject.end();\
		++x_iterator)

#define sxForEachC(x_iterator, x_collectionObject, x_collectionType)\
	for(x_collectionType::const_iterator x_iterator = x_collectionObject.begin();\
		x_iterator != x_collectionObject.end();\
		++x_iterator)

// for-like reverse iteration
#define sxForEachReverse(x_iterator, x_collectionObject, xcollectionType)\
	for(xcollectionType::reverse_iterator x_iterator = x_collectionObject.rbegin();\
		x_iterator != x_collectionObject.rend();\
		++x_iterator)
#define sxForEachReverseC(x_iterator, x_collectionObject, xcollectionType)\
	for(xcollectionType::const_reverse_iterator x_iterator = x_collectionObject.rbegin();\
		x_iterator != x_collectionObject.rend();\
		++x_iterator)

// Function calling iteration
#define sxForEachCallFunctor(x_collectionObject, x_Functor)\
	for_each(	x_collectionObject.begin(),\
				x_collectionObject.end(),\
				x_Functor);

// Function calling reverse iteration
#define sxForEachReverseCallFunctor(x_collectionObject, x_Functor)\
	for_each(	x_collectionObject.rbegin(),\
				x_collectionObject.rend(),\
				x_Functor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Helper enum declaration and iteration macro.

#define sxEnumBegin(x_enumType)\
struct x_enumType\
{\
	enum sxEValue\
	{

#define sxEnumEnd()\
		eValuesCount,\
	};\
};

// Iterate from begin to end of an enum
#define sxForEachEnum(x_iterValue, x_enumType)\
	for(x_enumType::sxEValue x_iterValue = x_enumType::sxEValue(0);\
		x_iterValue < x_enumType::eValuesCount;\
		x_iterValue = x_enumType::sxEValue(x_iterValue + 1))

// Iterate in a range of an enum
#define sxForEachEnumInRange(x_iterValue, x_enumMin, x_enumMax, x_enumType)\
	for(x_enumType::sxEValue x_iterValue = x_enumType::sxEValue(x_enumType::x_enumMin);\
		x_iterValue < x_enumType::x_enumMax;\
		x_iterValue = x_enumType::sxEValue(x_iterValue + 1))

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Helper integer iteration macro
#define sxForEachUInt(x_uiIterator, x_uiMax)\
	for(sxUInt x_uiIterator = 0, x_uiIterator##Max = x_uiMax;\
		x_uiIterator < x_uiIterator##Max;\
		++x_uiIterator)
#define sxForEachUIntInRange(x_uiIterator, x_uiMin, x_uiMax)\
	for(sxUInt x_uiIterator = x_uiMin, x_uiIterator##Max = x_uiMax;\
		x_uiIterator < x_uiIterator##Max;\
		++x_uiIterator)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sx includes (second part)
#include "sxRegistration.h"
#include "sxITSingleton.h"
#include "sxLog.h"
#include "Thread/sxCAtomicInt.h"
#include "Math/sxMath.h"
#include "UnitTest/sxUnitTest.h"
#include "sxCTSmartPtr.h"
#include "sxCTime.h"
