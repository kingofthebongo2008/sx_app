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

#ifdef __sxBuildUTest

#include "sxDriver/Server/Interface/sxIServerCommand.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UTest server command object. Used as the UTest specialisation of sxCUTestCommand.
class sxCUTestServerCommand : public sxIServerCommand
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UTest server chacke command object. Used as the UTest specialisation of sxCUTestCheckCommand. 
class sxCUTestCheckServerCommand : public sxIServerCommand
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Set the value that will be tested against the internal built value.
	static void SetValueToTest(sxUInt a_uiValue) { ms_uiValueToTest = a_uiValue; }

protected:

	//-------------------------------------------------------------------------------------------------------
	// Build according to a new data
	virtual void Build(sxICommandData const& a_rData, class sxIServerDevice& a_rDevice);

	//-------------------------------------------------------------------------------------------------------
	// Dispatch to specific API. Check that a_rData value is equal to ms_uiValueToTest
	virtual void Dispatch(sxICommandData const& a_rData, class sxIServerDevice& a_rDevice);

private:

	// Internal build data value
	sxUInt m_uiBuiltValue;

	// Value to test against the built data value. ms_uiValueToTest is shared among all instances so the test
	// only works againts the last dispatched sxCUTestCheckCommand.
	static sxUInt ms_uiValueToTest;
};

#endif // __sxBuildUTest