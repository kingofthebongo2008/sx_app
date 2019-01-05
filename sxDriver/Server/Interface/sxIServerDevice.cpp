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
#include "sxIServerDevice.h"
#include "sxDriver/Client/sxCDevice.h"

//-------------------------------------------------------------------------------------------------------
//
sxIServerDevice::sxIServerDevice() :
	m_iaResetNeeded(0)
{
}

//-------------------------------------------------------------------------------------------------------
//
sxIServerDevice::~sxIServerDevice()
{
}

//-------------------------------------------------------------------------------------------------------
//
void sxIServerDevice::ResetNeeded()
{
	// Flag reset as needed
	m_iaResetNeeded = 1;
}

//-------------------------------------------------------------------------------------------------------
//
sxBool sxIServerDevice::IsResetNeeded() const
{
	return (m_iaResetNeeded == 1);
}

//-------------------------------------------------------------------------------------------------------
//
sxBool sxIServerDevice::Reset()
{
	sxAssert(IsResetNeeded(), "Reset is not required");

	// Flag as reseted
	Reseted();

	return true;
}

//-------------------------------------------------------------------------------------------------------
//
void sxIServerDevice::Reseted()
{
	sxAssert(IsResetNeeded(), "Reset was not required");

	// Reset is not required anymore
	m_iaResetNeeded = 0;

	sxLog("Driver server reseted");
}