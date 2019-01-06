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

#include "sxDriver/Client/sxICommand.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Swap command data.
class sxCSwapCommandData final : public sxICommandData
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor.
	sxCSwapCommandData()
	{
	}

	//-------------------------------------------------------------------------------------------------------
	// Implement virtual clear function
	virtual void Clear()
	{
	}

	//-------------------------------------------------------------------------------------------------------
	// Implement virtual copy function
	virtual void Copy(sxICommandData const& a_rData)
	{
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Type of the sxCUTestCommand object built base on the command wrapper helper
typedef sxCTCommandWrapper<sxCSwapCommandData, sxEFactoryObject::eSwapCommand> sxCSwapCommand;
