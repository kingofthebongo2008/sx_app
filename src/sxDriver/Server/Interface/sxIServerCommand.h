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

#include "sxIServerObject.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Driver server command object interface
class sxIServerCommand : public sxIServerObject
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxIServerCommand() {}

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxIServerCommand() {}

protected:

private:

	// sxCPacket is friend in order to call Build and Dispatch functions
	friend class sxCPacket;

	//-------------------------------------------------------------------------------------------------------
	// Build according to a new data. Can be overloaded by the server implementation
	virtual void Build(	class sxICommandData const& a_rData,
						class sxIServerDevice& a_rDevice)
	{
	}

	//-------------------------------------------------------------------------------------------------------
	// Dispatch to specific API. Must be overloaded by the server implementation
	virtual void Dispatch(	class sxICommandData const& a_rData,
							class sxIServerDevice& a_rDevice) = 0;
};