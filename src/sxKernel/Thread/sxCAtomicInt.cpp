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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Thread unit tests
#ifdef __sxBuildUTest

//-----------------------------------------------------------------------------------------------------------
//
void UTestAtomic()
{
    sxCAtomicInt iValue(12);

    // Comparison operators
    sxUTest(iValue == 12);
    sxUTest(iValue != 21);

    // Increment
    sxUTest(++iValue == 13);
    sxUTest(iValue++ == 13);
    sxUTest(iValue == 14);

    // Decrement
    sxUTest(--iValue == 13);
    sxUTest(iValue-- == 13);
    sxUTest(iValue == 12);

    // Exchange
    iValue = 21;
    sxUTest(iValue == 21);

    // CompareExchange
    sxUTest(iValue.CompareExchange(21, 12) == 21);
    sxUTest(iValue == 12);
    sxUTest(iValue.CompareExchange(21, 0) == 12);
    sxUTest(iValue == 12);

    // Operators
    sxUTest(iValue >= 12);
    sxUTest(iValue > 11);
    sxUTest(iValue == 12);
    sxUTest(iValue <= 12);
    sxUTest(iValue < 13);
    sxUTest(iValue != 135133);
}

// Register test
sxRegisterUTestCase(UTestAtomic, sxERegisterSecondaryPriority::eHigh, "Atomic integers");

#endif // __sxBuildUTest
