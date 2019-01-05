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

#ifdef __sxBuildUTest

#include "sxCUTestFactory.h"
#include "sxCUTestDevice.h"
#include "Command/sxCUTestServerCommand.h"
#include "State/sxCUTestRenderTargetState.h"
#include "State/sxCUTestDefaultServerState.h"

//-----------------------------------------------------------------------------------------------------------
// Instantiate and register sxCUTestFactory object
sxRegisterServerFactory(sxCUTestFactory);

//-----------------------------------------------------------------------------------------------------------
//
sxCUTestFactory::sxCUTestFactory() :
	sxITServerFactoryWrapper<sxCUTestFactory>(sxEGraphicApi::eUTest)
{
}

//-----------------------------------------------------------------------------------------------------------
//
sxCUTestFactory::~sxCUTestFactory()
{
}

//-----------------------------------------------------------------------------------------------------------
//
sxIServerObject& sxCUTestFactory::Instantiate(sxEFactoryObject::sxEValue a_eValue)
{
	switch(a_eValue)
	{
		case sxEFactoryObject::eDevice: return sxNew(sxCUTestDevice);

		case sxEFactoryObject::eUTestCommand: return sxNew(sxCUTestServerCommand);
		case sxEFactoryObject::eUTestCheckCommand: return sxNew(sxCUTestCheckServerCommand);

		// Instantiate the same state by default for all the real state types
		case sxEFactoryObject::eRenderTargetState:
			return sxNew(sxCUTestRenderTargetState);

		case sxEFactoryObject::eVertexBufferState:
		case sxEFactoryObject::eTopologyState:
		case sxEFactoryObject::eMaterialState:
		case sxEFactoryObject::eLightState:
		case sxEFactoryObject::eCameraState:
		case sxEFactoryObject::eTransformState:
			return sxNew(sxCUTestDefaultServerState);

		default: return sxGetInvalidReference<sxIServerObject>();
	}
}

#endif // __sxBuildUTest
