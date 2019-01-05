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

#ifdef __sxBuildUTest

#include "sxDriver/Server/Interface/sxIServerDevice.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Driver UTest server renderer
class sxCUTestDevice :	public sxIServerDevice,
						public sxITSingleton<sxCUTestDevice>
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxCUTestDevice();

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxCUTestDevice();

	//----------------------------------------------------------------------------------------------------------
	// Ask for a device reset
	void ForceResetNeeded();
};

#endif // __sxBuildUTest
