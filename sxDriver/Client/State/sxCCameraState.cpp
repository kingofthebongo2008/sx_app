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
#include "sxCCameraState.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Register sxCCameraState default constructor
sxRegisterDefaultState(sxCCameraState, sxEStateType::eCamera);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCCameraStateData implementation

//-------------------------------------------------------------------------------------------------------
//
sxCCameraStateData::sxCCameraStateData() :
	m_f3Eye(0, 0, 0),
	m_f3At(0, 0, 0),
	m_f3Up(0, 1, 0),
	m_fFocalAngle(90),
	m_fRatio(4.f/3.f),
	m_fNear(0.1f), m_fFar(100.f)
{
}

//-------------------------------------------------------------------------------------------------------
//
void sxCCameraStateData::Clear()
{
	m_f3Eye = sxCFloat3(0, 0, 0);
	m_f3At = sxCFloat3(0, 0, 0);
	m_f3Up = sxCFloat3(0, 1, 0);
	
	m_fFocalAngle = 90;
	
	m_fRatio = 4.f/3.f;

	m_fNear = 0.1f;
	m_fFar = 100.f;
}

//-------------------------------------------------------------------------------------------------------
//
void sxCCameraStateData::Copy(sxICommandData const& a_rData)
{
	sxCCameraStateData const& rData = (sxCCameraStateData const&)a_rData;

	m_f3Eye = rData.m_f3Eye;
	m_f3At = rData.m_f3At;
	m_f3Up = rData.m_f3Up;
	
	m_fFocalAngle = rData.m_fFocalAngle;
	
	m_fRatio = rData.m_fRatio;

	m_fNear = rData.m_fNear;
	m_fFar = rData.m_fFar;
}
