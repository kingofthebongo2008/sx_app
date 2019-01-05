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

#ifdef __sxBuildLog
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enabled logging implementation

//-----------------------------------------------------------------------------------------------------------
// Logging maco definition
#define sxLog(...) sxNsLog::Log(__VA_ARGS__)

//-----------------------------------------------------------------------------------------------------------
// Internal function
namespace sxNsLog
{
	void Log(sxChar const* szFormat, ...);
}

#else // __sxBuildLog

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Disabled logging implementation

//-----------------------------------------------------------------------------------------------------------
// Logging maco definition
#define sxLog(...)

#endif // __sxBuildLog