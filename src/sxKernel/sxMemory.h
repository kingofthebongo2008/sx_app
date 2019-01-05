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

#include "sxType.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// C like allocation functions

// Default alignement value
static sxSizeT const sxstAlignementDefault = 16;

//-----------------------------------------------------------------------------------------------------------
// Allocate a buffer of a_stSize. 
void* sxMalloc(sxSizeT a_stSize, sxSizeT a_stAlignement = sxstAlignementDefault);

//-----------------------------------------------------------------------------------------------------------
// Allocate a buffer of a_stSize. 
void* sxRealloc(void* a_pvAlloc, sxSizeT a_stSize, sxSizeT a_stAlignement = sxstAlignementDefault);

//-----------------------------------------------------------------------------------------------------------
// Free a_pvAlloc buffer
void sxFree(void* a_pvAlloc);

//-----------------------------------------------------------------------------------------------------------
// new operator replacement macro
#define sxNew(x_Type, ...) *new x_Type(__VA_ARGS__)
#define sxNewArray(x_Type, x_uiCount) new x_Type[x_uiCount]

//-----------------------------------------------------------------------------------------------------------
// Delete operator implementation

namespace sxNsMemory
{

// Delete (reference) internal function implementation
template <typename t_Object>
inline void sxDelete(t_Object& a_rObject)
{
	delete &a_rObject;
}

// Delete (pointer) internal function implementation
template <typename t_Object>
inline void sxDelete(t_Object*)
{
    static_assert(false);
	//sxStaticAssert(false, sxDelete_should_not_be_called_on_a_pointer_unless_it_is_an_array);
}

} // sxNsMemory

//-----------------------------------------------------------------------------------------------------------
// sx delete operator replacement macro
#define sxDelete(x_Object) sxNsMemory::sxDelete(x_Object)
#define sxDeleteArray(x_Object) delete [] x_Object

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Memory operations

//-----------------------------------------------------------------------------------------------------------
// Copy a_stSize bytes of a_pvSource to a_pvDestination
void sxMemCopy(void* a_pvDestination, void const* a_pvSource, sxSizeT a_stSize);

//-----------------------------------------------------------------------------------------------------------
// Set a_stSize bytes of a_pvDestination with a_sCharacter
void sxMemSet(void* a_pvDestination, sxChar a_cCharacter, sxSizeT a_stSize);

//-----------------------------------------------------------------------------------------------------------
// Set a_stSize bytes of a_pvDestination with zeros
inline void sxMemZero(void* a_pvDestination, sxSizeT a_stSize)
{
	sxMemSet(a_pvDestination, 0, a_stSize);
}

//-----------------------------------------------------------------------------------------------------------
// Set a_rObject with zeros
template <typename t_Object>
inline void sxMemZero(t_Object& a_rObject)
{
	sxMemZero(&a_rObject, sizeof(t_Object));
}
