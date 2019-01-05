//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
//                                                                                                          |
//                              ShaderX7. Cross platform rendering thread. 2008.                            |
//              Guillaume Blanc. ELB (Etranges Libellules) http://www.elb-games.com/. Lyon, France.         |
//                                                                                                          |
//  This program is free software. It is distributed in the hope that it will be useful, but without any    |
//  warranty, without even the implied warranty of merchantability or fitness for a particular purpose.     |
//                                                                                                          |
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
#include "pch.h"
#include "sxCD3d9CameraState.h"
#include "sxDriver/Server/D3d9/sxCD3d9Device.h"
#include "sxDriver/Client/State/sxCCameraState.h"

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9CameraState::Build( sxICommandData const& a_rData, sxIServerDevice& a_rDevice)
{
    // Fetch render target state data
    sxCCameraStateData const& rData = (sxCCameraStateData const&)a_rData;

    //------------------
    // Build view matrix

    // Get eye position from the data
    sxCFloat3 const& rf3Eye = rData.GetEye();
    D3DXVECTOR3 oEye(rf3Eye.m_fX, rf3Eye.m_fY, rf3Eye.m_fZ);

    // Get at position from the data
    sxCFloat3 const& rf3At = rData.GetAt();
    D3DXVECTOR3 oAt(rf3At.m_fX, rf3At.m_fY, rf3At.m_fZ);

    // Get up direction from the data
    sxCFloat3 const& rf3Up = rData.GetUp();
    D3DXVECTOR3 oUp(rf3Up.m_fX, rf3Up.m_fY, rf3Up.m_fZ);

    // Initialize the matrix
    D3DXMatrixLookAtLH( &m_oViewMatrix,
                        &oEye,
                        &oAt,
                        &oUp);

    //------------------
    // Build projection matrix
    D3DXMatrixPerspectiveFovLH( &m_oProjectionMatrix,
                                D3DXToRadian(rData.GetFocalAngle()),
                                rData.GetRatio(),
                                rData.GetNear(),
                                rData.GetFar());
}

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9CameraState::Dispatch(sxICommandData const& a_rData, sxIServerDevice& a_rDevice)
{
    // Fetch D3d9 device
    sxCD3d9Device& rDevice = (sxCD3d9Device&)a_rDevice;

    // Update shadow. Should be done in the server generic code rather than per state.
    rDevice.GetStateShadow().SetState(sxEStateType::eCamera, rThis, a_rData);

    // Set the view matrix
    sxD3d9Call(rDevice.GetD3dDevice().SetTransform(D3DTS_VIEW, &m_oViewMatrix));

    // Set the projection matrix
    sxD3d9Call(rDevice.GetD3dDevice().SetTransform(D3DTS_PROJECTION, &m_oProjectionMatrix));
}
