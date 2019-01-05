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

#include "sxIObject.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Driver client renderer
class sxCDevice : protected sxIObject
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxCDevice();

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxCDevice();

	//-------------------------------------------------------------------------------------------------------
	// Execute device reset. This function is executed synchronously on the server side, so the device must
	// have been properly stopped.
	sxBool Reset();

	//--------------------------------------------------------------------------------------------------------
	// Test if a reset is required. Forward the question to the server side.
	sxBool IsResetNeeded() const;

private:

	// sxCPacket have access to the GetServerDevice function in order to execute commands on the server.
	friend class sxCPacket;

	// Private access to the server side
	class sxIServerDevice& GetServerDevice() const;
};