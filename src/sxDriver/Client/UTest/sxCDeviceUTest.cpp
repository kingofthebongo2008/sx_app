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

#include "sxDriver/Client/sxCDevice.h"
#include "sxDriver/Server/UTest/sxCUTestDevice.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// State cache unit tests
#ifdef __sxBuildUTest

//-----------------------------------------------------------------------------------------------------------
//
void UTestDeviceReset()
{
	sxCFactoryRaiiSelector oRaiiSelector(sxEGraphicApi::eUTest);

	// Instantiate a device
	sxCDevice oDevice;

	// Reset isn't need by default
	sxUTest(oDevice.IsResetNeeded() == false);

	// Reset isn't allowed if not asked from the server
	sxUTestAssert(oDevice.Reset());

	// Ask the server device to force a reset
	sxCUTestDevice::GetInstance().ForceResetNeeded();

	// Reset should now be required
	sxUTest(oDevice.IsResetNeeded() == true);

	// Lets do it
	sxUTest(oDevice.Reset());

	// Reset should now be done
	sxUTest(oDevice.IsResetNeeded() == false);
}

// Register test
sxRegisterUTestCase(UTestDeviceReset,
					sxERegisterSecondaryPriority::eBelowNormal,
					"Device reset");

#endif // __sxBuildUTest
