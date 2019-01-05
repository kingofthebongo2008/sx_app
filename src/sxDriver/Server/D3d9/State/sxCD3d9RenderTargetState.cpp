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
#include "sxCD3d9RenderTargetState.h"
#include "sxDriver/Server/D3d9/sxCD3d9Device.h"
#include "sxDriver/Client/State/sxCRenderTargetState.h"

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9RenderTargetState::Build( sxICommandData const& a_rData, sxIServerDevice& a_rDevice)
{
    // Fetch render target state data
    sxCRenderTargetStateData const& rData = (sxCRenderTargetStateData const&)a_rData;

    // Get color from the data
    sxCFloat4 f4Color = rData.GetColor();

    // Build the D3d color object
    m_oColor = D3DCOLOR_COLORVALUE(f4Color.m_fX, f4Color.m_fY, f4Color.m_fZ, f4Color.m_fW);
}

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9RenderTargetState::Dispatch(sxICommandData const& a_rData, sxIServerDevice& a_rDevice)
{
    // Fetch D3d9 device
    sxCD3d9Device& rDevice = (sxCD3d9Device&)a_rDevice;

    // Update shadow. Should be done in the server generic code rather than per state.
    rDevice.GetStateShadow().SetState(sxEStateType::eRenderTarget, rThis, a_rData);

    // Fetch render target state data
    sxCRenderTargetStateData const& rData = (sxCRenderTargetStateData const&)a_rData;

    //-------------------------------------------------------------
    // Forward current render target width and height to the device
    rDevice.SetDeviceWidth(rData.GetWidth());
    rDevice.SetDeviceHeight(rData.GetHeight());

    //-------------------------
    // Clear this render target

    // Begin a new scene (can be called multiple times)
    rDevice.BeginScene();
    
    // Call the d3d9 clear function
    sxD3d9Call(rDevice.GetD3dDevice().Clear(0,
                                            NULL,
                                            D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                                            m_oColor,
                                            1,
                                            0));
}
