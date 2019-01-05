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
#include "sxCD3d9TransformState.h"
#include "sxDriver/Server/D3d9/sxCD3d9Device.h"
#include "sxDriver/Client/State/sxCTransformState.h"

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9TransformState::Build( sxICommandData const& a_rData, sxIServerDevice& a_rDevice)
{
	// Fetch render target state data
	sxCTransformStateData const& rData = (sxCTransformStateData const&)a_rData;

	// Copy to the D3d9 matrix object

	sxCFloat3 const& rRight = rData.GetRight();
	m_oWorldMatrix.m[0][0] = rRight.m_fX;
	m_oWorldMatrix.m[0][1] = rRight.m_fY;
	m_oWorldMatrix.m[0][2] = rRight.m_fZ;
	m_oWorldMatrix.m[0][3] = 0;

	sxCFloat3 const& rUp = rData.GetUp();
	m_oWorldMatrix.m[1][0] = rUp.m_fX;
	m_oWorldMatrix.m[1][1] = rUp.m_fY;
	m_oWorldMatrix.m[1][2] = rUp.m_fZ;
	m_oWorldMatrix.m[1][3] = 0;

	sxCFloat3 const& rAt = rData.GetAt();
	m_oWorldMatrix.m[2][0] = rAt.m_fX;
	m_oWorldMatrix.m[2][1] = rAt.m_fY;
	m_oWorldMatrix.m[2][2] = rAt.m_fZ;
	m_oWorldMatrix.m[2][3] = 0;

	sxCFloat3 const& rPos = rData.GetPos();
	m_oWorldMatrix.m[3][0] = rPos.m_fX;
	m_oWorldMatrix.m[3][1] = rPos.m_fY;
	m_oWorldMatrix.m[3][2] = rPos.m_fZ;
	m_oWorldMatrix.m[3][3] = 1;
}

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9TransformState::Dispatch(sxICommandData const& a_rData, sxIServerDevice& a_rDevice)
{
	// Fetch D3d9 device
	sxCD3d9Device& rDevice = (sxCD3d9Device&)a_rDevice;

	// Update shadow. Should be done in the server generic code rather than per state.
	rDevice.GetStateShadow().SetState(sxEStateType::eTransform, rThis, a_rData);

	// Set the m_oWorldMatrix as the current world matrix
	sxD3d9Call(rDevice.GetD3dDevice().SetTransform(D3DTS_WORLD, &m_oWorldMatrix));
}
