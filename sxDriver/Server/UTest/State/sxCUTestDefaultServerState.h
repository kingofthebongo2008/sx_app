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
// UTest server state object. Used as the UTest specialisation of all client state types.
class sxCUTestDefaultServerState : public sxIServerState
{
protected:

	//-------------------------------------------------------------------------------------------------------
	// Build according to a new data
	virtual void Build(sxICommandData const& a_rData, class sxIServerDevice& a_rDevice) {}

	//-------------------------------------------------------------------------------------------------------
	// Dispatch to specific API
	virtual void Dispatch(sxICommandData const& a_rData, class sxIServerDevice& a_rDevice) {}

private:

};

#endif // __sxBuildUTest