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
#include "sxCFloat3.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Thread unit tests
#ifdef __sxBuildUTest

//-----------------------------------------------------------------------------------------------------------
//
void UTestFloat3()
{
    sxCFloat3 f3Uninit;

    // Assignment test
    sxCFloat3 f3Init(0, 1, 2);
    sxUTest(f3Init.m_fX == 0 && f3Init.m_fY == 1 && f3Init.m_fZ == 2 );

    sxCFloat3 m_f3Copy(f3Init);
    sxUTest(m_f3Copy.m_fX == 0 && m_f3Copy.m_fY == 1 && m_f3Copy.m_fZ == 2);

    sxCFloat3 m_f3Assign;
    m_f3Assign = f3Init;
    sxUTest(m_f3Assign.m_fX == 0 && m_f3Assign.m_fY == 1 && m_f3Assign.m_fZ == 2);

    // Comparison tests
    sxUTest(m_f3Assign == m_f3Copy);
    sxUTest(m_f3Assign != f3Uninit);
}

// Register test
sxRegisterUTestCase(UTestFloat3, sxERegisterSecondaryPriority::eHigh, "Float3");

#endif // __sxBuildUTest
