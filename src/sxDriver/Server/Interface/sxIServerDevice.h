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
// Driver server renderer interface
class sxIServerDevice :	public sxIServerObject
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxIServerDevice();

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxIServerDevice();

	//-------------------------------------------------------------------------------------------------------
	// Request a reset
	void ResetNeeded();

protected:

	//-------------------------------------------------------------------------------------------------------
	// Flag the reset as done
	void Reseted();

private:

	// sxCDevice is friend in order to access reset private functions
	friend class sxCDevice;

	//-------------------------------------------------------------------------------------------------------
	// Test if the reset is needed
	sxBool IsResetNeeded() const;

	//----------------------------------------------------------------------------------------------------------
	// Reset virtual function
	virtual sxBool Reset();

	//-------------------------------------------------------------------------------------------------------
	// Attributes

	// Stores if the reset has be asked using an atomic integer as reset flag is tested and written from the 
	// client and the server threads.
	sxCAtomicInt m_iaResetNeeded;
};
