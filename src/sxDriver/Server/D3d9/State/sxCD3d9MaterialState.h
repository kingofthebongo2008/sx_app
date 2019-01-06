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

#include "sxDriver/Server/Interface/sxIServerState.h"
#include "sxDriver/Server/D3d9/sxD3d9.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Material d3d9 server state object. Used as the d3d9 specialisation of sxCMaterialState.
class sxCD3d9MaterialState final : public sxIServerState
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxCD3d9MaterialState();

protected:
	
	//-------------------------------------------------------------------------------------------------------
	// Build according to a new data
	virtual void Build(sxICommandData const& a_rData, class sxIServerDevice& a_rDevice);

	//-------------------------------------------------------------------------------------------------------
	// Dispatch to specific API
	virtual void Dispatch(sxICommandData const& a_rData, class sxIServerDevice& a_rDevice);

private:

	//-------------------------------------------------------------------------------------------------------
	// Attributes

	// D3d material object, built from the data
	D3DMATERIAL9 m_oD3d9Material;

	// Should specular lighting be enabled
	sxBool m_bSpecualLightingEnabled;
};
