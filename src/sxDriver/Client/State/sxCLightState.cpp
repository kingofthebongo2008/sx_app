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
#include "sxCLightState.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Register sxCLightState default constructor
sxRegisterDefaultState(sxCLightState, sxEStateType::eLight);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCLight implementation

//-------------------------------------------------------------------------------------------------------
//
sxCLight::sxCLight() :
	m_f3Color(0 ,0, 0),
	m_f3Position(0, 0, 0),
	m_fAmbient(0),
	m_fDiffuse(0),
	m_fSpecular(0),
	m_fAttenuation(0)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCLightStateData implementation

//-------------------------------------------------------------------------------------------------------
//
sxCLightStateData::sxCLightStateData()
{
}

//-------------------------------------------------------------------------------------------------------
//
void sxCLightStateData::Clear()
{
	m_vLights.clear();
}

//-------------------------------------------------------------------------------------------------------
//
void sxCLightStateData::Copy(sxICommandData const& a_rData)
{
	sxCLightStateData const& rData = (sxCLightStateData const&)a_rData;

	m_vLights = rData.m_vLights;
}
