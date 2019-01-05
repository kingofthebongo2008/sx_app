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
#include "sxCFloat4.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Thread unit tests
#ifdef __sxBuildUTest

//-----------------------------------------------------------------------------------------------------------
//
void UTestFloat4()
{
    sxCFloat4 f4Uninit;

    // Assignment test
    sxCFloat4 f4Init(0, 1, 2, 3);
    sxUTest(f4Init.m_fX == 0 && f4Init.m_fY == 1 && f4Init.m_fZ == 2 && f4Init.m_fW == 3);

    sxCFloat4 m_f4Copy(f4Init);
    sxUTest(m_f4Copy.m_fX == 0 && m_f4Copy.m_fY == 1 && m_f4Copy.m_fZ == 2 && m_f4Copy.m_fW == 3);

    sxCFloat4 m_f4Assign;
    m_f4Assign = f4Init;
    sxUTest(m_f4Assign.m_fX == 0 && m_f4Assign.m_fY == 1 && m_f4Assign.m_fZ == 2 && m_f4Assign.m_fW == 3);

    // Comparison tests
    sxUTest(m_f4Assign == m_f4Copy);
    sxUTest(m_f4Assign != f4Uninit);
}

// Register test
sxRegisterUTestCase(UTestFloat4, sxERegisterSecondaryPriority::eHigh, "Float4");

#endif // __sxBuildUTest
