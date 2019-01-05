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
// Render target d3d9 server state object. Used as the d3d9 specialisation of sxCRenderTargetState.
class sxCD3d9RenderTargetState : public sxIServerState
{
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

	// D3d color object, built from the data
	D3DCOLOR m_oColor;
};
