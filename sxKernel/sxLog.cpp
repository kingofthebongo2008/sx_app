//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
//																											|
//								ShaderX7. Cross platform rendering thread. 2008.							|
//				Guillaume Blanc. ELB (Etranges Libellules) http://www.elb-games.com/. Lyon, France.			|
//																											|
//	This program is free software. It is distributed in the hope that it will be useful, but without any	|
//	warranty, without even the implied warranty of merchantability or fitness for a particular purpose.		|
//																											|
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
#include "sxKernel/sxPcHeader.h"

#include <windows.h>

#ifdef __sxBuildLog

namespace sxNsLog
{
//-----------------------------------------------------------------------------------------------------------
//
void Log(sxChar const* szFormat, ...)
{
	// Retrieve the variable arguments
	va_list vaArgs;
	va_start(vaArgs, szFormat);

	// Compute message size
	sxSizeT stLen = _vscprintf(szFormat, vaArgs) + 1 + 1;	// _vscprintf doesn't count terminating '\0'
															// We also add a \n character to force flushing
	// Allocate message buffer
	sxChar* szBuffer = (sxChar*)sxMalloc(stLen * sizeof(sxChar));

	// Format message
	vsprintf_s(szBuffer, stLen, szFormat, vaArgs);
	strcat_s(szBuffer, stLen, "\n");

	// Output message to debugger
	OutputDebugStringA(szBuffer);

	// Output message to console
	printf(szBuffer);

	// Free message
	sxFree(szBuffer);
}
}

#endif // __sxBuildLog