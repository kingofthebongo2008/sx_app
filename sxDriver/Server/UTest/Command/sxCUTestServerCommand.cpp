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

#include "sxCUTestServerCommand.h"
#include "sxDriver/Client/Command/UTest/sxCUTestCommand.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCUTestServerCommand

//-----------------------------------------------------------------------------------------------------------
//
void sxCUTestServerCommand::Build(	sxICommandData const& a_rData,
									class sxIServerDevice& a_rDevice)
{
	sxCUTestCommandData const& rData = (sxCUTestCommandData const&)a_rData;

	m_uiBuiltValue = rData.GetValue();
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCUTestServerCommand::Dispatch(	sxICommandData const& a_rData,
										class sxIServerDevice& a_rDevice)
{
	sxCUTestCommandData const& rData = (sxCUTestCommandData const&)a_rData;

	sxUTest(m_uiBuiltValue == rData.GetValue());
	sxCUTestCheckServerCommand::SetValueToTest(m_uiBuiltValue);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCUTestCheckServerCommand
sxUInt sxCUTestCheckServerCommand::ms_uiValueToTest = 0;

//-----------------------------------------------------------------------------------------------------------
//
void sxCUTestCheckServerCommand::Build(	sxICommandData const& a_rData,
										class sxIServerDevice& a_rDevice)
{
	sxCUTestCheckCommandData const& rData = (sxCUTestCheckCommandData const&)a_rData;

	m_uiBuiltValue = rData.GetValue();
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCUTestCheckServerCommand::Dispatch(	sxICommandData const& a_rData,
											class sxIServerDevice& a_rDevice)
{
	sxCUTestCheckCommandData const& rData = (sxCUTestCheckCommandData const&)a_rData;

	sxUTest(m_uiBuiltValue == rData.GetValue());
	sxUTest(ms_uiValueToTest == m_uiBuiltValue);
}

#endif // __sxBuildUTest
