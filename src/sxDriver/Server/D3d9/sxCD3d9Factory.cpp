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
#include "sxCD3d9Factory.h"
#include "sxCD3d9Device.h"

// States
#include "State/sxCD3d9RenderTargetState.h"
#include "State/sxCD3d9VertexBufferState.h"
#include "State/sxCD3d9TopologyState.h"
#include "State/sxCD3d9MaterialState.h"
#include "State/sxCD3d9LightState.h"
#include "State/sxCD3d9CameraState.h"
#include "State/sxCD3d9TransformState.h"

// Commands
#include "Command/sxCD3d9SwapCommand.h"
#include "Command/sxCD3d9DrawCommand.h"

//-----------------------------------------------------------------------------------------------------------
// Instantiate and register sxCD3d9Factory object
sxRegisterServerFactory(sxCD3d9Factory);

//-----------------------------------------------------------------------------------------------------------
//
sxCD3d9Factory::sxCD3d9Factory() :
	sxITServerFactoryWrapper<sxCD3d9Factory>(sxEGraphicApi::eD3d9)
{
}

//-----------------------------------------------------------------------------------------------------------
//
sxCD3d9Factory::~sxCD3d9Factory()
{
}

//-----------------------------------------------------------------------------------------------------------
//
sxIServerObject& sxCD3d9Factory::Instantiate(sxEFactoryObject::sxEValue a_eValue)
{
	switch(a_eValue)
	{
		case sxEFactoryObject::eDevice: return sxNew(sxCD3d9Device);

		case sxEFactoryObject::eRenderTargetState: return sxNew(sxCD3d9RenderTargetState);
		case sxEFactoryObject::eVertexBufferState: return sxNew(sxCD3d9VertexBufferState);
		case sxEFactoryObject::eTopologyState: return sxNew(sxCD3d9TopologyState);
		case sxEFactoryObject::eMaterialState: return sxNew(sxCD3d9MaterialState);
		case sxEFactoryObject::eLightState: return sxNew(sxCD3d9LightState);
		case sxEFactoryObject::eCameraState: return sxNew(sxCD3d9CameraState);
		case sxEFactoryObject::eTransformState: return sxNew(sxCD3d9TransformState);

		case sxEFactoryObject::eSwapCommand: return sxNew(sxCD3d9SwapCommand);
		case sxEFactoryObject::eDrawCommand: return sxNew(sxCD3d9DrawCommand);

		default: return sxGetInvalidReference<sxIServerObject>();
	}
}



