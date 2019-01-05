//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
//																											|
//						Guillaume Blanc. ELB (Etranges Libellules). Lyon FRANCE. 2008.						|
//								ShaderX7. Cross platform rendering thread									|
//																											|
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
#pragma once

#ifdef __sxBuildUTest

#include "sxDriver/Server/Interface/sxIServerState.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UTest server render target state object. Used as the UTest specialisation of sxCRenderTargetState.
class sxCUTestRenderTargetState : public sxIServerState
{
protected:

	//-------------------------------------------------------------------------------------------------------
	// Build according to a new data
	virtual void Build(sxICommandData const& a_rData, class sxIServerDevice& a_rDevice);

	//-------------------------------------------------------------------------------------------------------
	// Dispatch to specific API
	virtual void Dispatch(sxICommandData const& a_rData, class sxIServerDevice& a_rDevice);

private:

	// Built value to test
	sxUInt m_uiBuiltValue;
};

#endif // __sxBuildUTest
