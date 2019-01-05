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
#include <windows.h>

#ifdef __sxBuildAssert

namespace sxNsAssertion
{

#ifdef __sxBuildUTest
__declspec(thread) sxBool sxCUTestAssertionEnabler::ms_blsEnabled = false;
#endif // __sxBuildUTest

//-----------------------------------------------------------------------------------------------------------
// 
sxBool sxAssertionFailureReport(sxChar const* szFormat, ...)
{
#ifdef __sxBuildUTest
    if(sxCUTestAssertionEnabler::IsEnabled())
    {
        throw sxCUTestAssertionException();
    }
#endif // __sxBuildUTest

    // Retrieve the variable arguments
    va_list vaArgs;
    va_start(vaArgs, szFormat);
    
    // Compute message size
    sxSizeT stLen = _vscprintf(szFormat, vaArgs) + 1;   // _vscprintf doesn't count terminating '\0'

    // Allocate message buffer
    sxChar* szBuffer = (sxChar*)sxMalloc(stLen * sizeof(sxChar));

    // Format message
    vsprintf_s(szBuffer, stLen, szFormat, vaArgs);

    // Output message to debugger
    sxLog(szBuffer);

    // Display the assertion and get user's clicked button
    sxInt iButton = MessageBoxA(NULL, szBuffer, "sxApplication: Assertion failed", MB_ABORTRETRYIGNORE);

    // Free message
    sxFree(szBuffer);

    // Do appropriate action according to user's choice
    switch(iButton)
    {
        case IDABORT: exit(EXIT_FAILURE);   // Exit application
        case IDIGNORE: return false;        // Continue execution
        default: return true;               // Generate a breakpoint
    }
}

} // sxNsAssertion

#endif // __sxBuildAssert
