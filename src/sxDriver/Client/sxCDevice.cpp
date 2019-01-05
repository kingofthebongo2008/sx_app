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
#include "sxCDevice.h"
#include "sxDriver/Server/Interface/sxIServerDevice.h"

//-----------------------------------------------------------------------------------------------------------
//
sxCDevice::sxCDevice() :
	sxIObject(sxEFactoryObject::eDevice)
{
}

//-----------------------------------------------------------------------------------------------------------
//
sxCDevice::~sxCDevice()
{
}

//-----------------------------------------------------------------------------------------------------------
//
sxIServerDevice& sxCDevice::GetServerDevice() const
{
	// Fetch server object
	sxIServerObject& rServerObject = GetServerObject();

	// Cast server object to sxIServerDevice
	return (sxIServerDevice&)rServerObject;
}

//-----------------------------------------------------------------------------------------------------------
//
sxBool sxCDevice::IsResetNeeded() const
{
	// Fetch server device
	sxIServerDevice& rServerDevice = GetServerDevice();

	// Forward to the server
	return rServerDevice.IsResetNeeded();
}

//-----------------------------------------------------------------------------------------------------------
//
sxBool sxCDevice::Reset()
{
	sxAssert(IsResetNeeded() == true, "Device reset isn't required");

	// Fetch server device
	sxIServerDevice& rServerDevice = (sxIServerDevice&)GetServerObject();

	// Forward reset
	return rServerDevice.Reset();
}
