//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
//                                                                                                          |
//                              ShaderX7. Cross platform rendering thread. 2008.                            |
//              Guillaume Blanc. ELB (Etranges Libellules) http://www.elb-games.com/. Lyon, France.         |
//                                                                                                          |
//  This program is free software. It is distributed in the hope that it will be useful, but without any    |
//  warranty, without even the implied warranty of merchantability or fitness for a particular purpose.     |
//                                                                                                          |
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
#include "pch.h"
#include "sxMemory.h"

#include <malloc.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Memory leak detection

#ifdef __sxBuildDebug

//-----------------------------------------------------------------------------------------------------------
// Registered function that perform leak detection
static void DetectMemoryLeak(sxERegisteredOperation::EValue a_eRegisteredOp)
{
    // Mem state at application boot time
    static _CrtMemState s_oBootMemState;

    if(a_eRegisteredOp == sxERegisteredOperation::eInitialize)
    {
        // Perform automatic leak checking at program exit 
        _CrtSetDbgFlag(_CrtSetDbgFlag(0) | _CRTDBG_LEAK_CHECK_DF);

        // Store the memory state at application boot time
        _CrtMemCheckpoint(&s_oBootMemState);
    }
    else if(a_eRegisteredOp == sxERegisteredOperation::eDestroy)
    {
        // Store the current memory state
        _CrtMemState oCurrMemState;
        _CrtMemCheckpoint(&oCurrMemState);

        // Compute memory state difference since boot
        _CrtMemState oDiffMemState;
        if(_CrtMemDifference(&oDiffMemState, &s_oBootMemState, &oCurrMemState))
        {
            // Dump all unfreed allocation since boot time
            _CrtMemDumpAllObjectsSince(&s_oBootMemState);
            
            // Inform user of the leaks
            sxAssert(false, "Memory leaks detected, see debugger output");
        }
    }
}

// Register the function
sxRegisterFunction(&DetectMemoryLeak,   sxERegisterPrimaryPriority::eReserved,
                                        sxERegisterSecondaryPriority::eHighest);

#endif // __sxBuildDebug

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// C like allocation functions

//-----------------------------------------------------------------------------------------------------------
//
void* sxMalloc(sxSizeT a_stSize, sxSizeT a_stAlignement)
{
    // Alocate the buffer
    void* pvAlloc = _aligned_malloc(a_stSize, a_stAlignement);

    // Allocation failure isn't supported. It works as an allocation of size 0 returns a valid pointer.
    sxAssert(pvAlloc != NULL, "Allocation failed");

    return pvAlloc;
}

//-----------------------------------------------------------------------------------------------------------
//
void* sxRealloc(void* a_pvAlloc, sxSizeT a_stSize, sxSizeT a_stAlignement)
{
    // Alocate the buffer
    void* pvAlloc = _aligned_realloc(a_pvAlloc, a_stSize, a_stAlignement);

    // Allocation failure isn't supported. It works as an allocation of size 0 returns a valid pointer.
    sxAssert(pvAlloc != NULL, "Allocation failed");

    return pvAlloc;
}

//-----------------------------------------------------------------------------------------------------------
//
void sxFree(void* a_pvAlloc)
{
    _aligned_free(a_pvAlloc);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Memory operations

//-----------------------------------------------------------------------------------------------------------
//
void sxMemCopy(void* a_pvDestination, void const* a_pvSource, sxSizeT a_stSize)
{
    memcpy(a_pvDestination, a_pvSource, a_stSize);
}

//-----------------------------------------------------------------------------------------------------------
//
void sxMemSet(void* a_pvDestination, sxChar a_cCharacter, sxSizeT a_stSize)
{
    memset(a_pvDestination, a_cCharacter, a_stSize);
}
