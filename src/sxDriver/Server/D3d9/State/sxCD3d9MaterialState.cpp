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
#include "sxCD3d9MaterialState.h"
#include "sxDriver/Server/D3d9/sxCD3d9Device.h"
#include "sxDriver/Client/State/sxCMaterialState.h"

//-------------------------------------------------------------------------------------------------------
//
sxCD3d9MaterialState::sxCD3d9MaterialState() :
    m_bSpecualLightingEnabled(false)
{
}

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9MaterialState::Build( sxICommandData const& a_rData, sxIServerDevice& a_rDevice)
{
    // Fetch material state data
    sxCMaterialStateData const& rData = (sxCMaterialStateData const&)a_rData;

    // Get color from the data
    sxCFloat4 f4Color = rData.GetColor();

    // Fills ambient value
    sxFloat const fAmbient = rData.GetAmbient();
    m_oD3d9Material.Ambient.r = f4Color.m_fX * fAmbient;
    m_oD3d9Material.Ambient.g = f4Color.m_fY * fAmbient;
    m_oD3d9Material.Ambient.b = f4Color.m_fZ * fAmbient;
    m_oD3d9Material.Ambient.a = f4Color.m_fW * fAmbient;

    // Fills diffuse value
    sxFloat const fDiffuse = rData.GetDiffuse();
    m_oD3d9Material.Diffuse.r = f4Color.m_fX * fDiffuse;
    m_oD3d9Material.Diffuse.g = f4Color.m_fY * fDiffuse;
    m_oD3d9Material.Diffuse.b = f4Color.m_fZ * fDiffuse;
    m_oD3d9Material.Diffuse.a = f4Color.m_fW * fDiffuse;

    // Fills specular value
    sxFloat const fSpecular = rData.GetSpecular();
    m_oD3d9Material.Specular.r = f4Color.m_fX * fSpecular;
    m_oD3d9Material.Specular.g = f4Color.m_fY * fSpecular;
    m_oD3d9Material.Specular.b = f4Color.m_fZ * fSpecular;
    m_oD3d9Material.Specular.a = f4Color.m_fW * fSpecular;

    // Fills emissive value
    sxFloat const fEmissive = rData.GetEmissive();
    m_oD3d9Material.Emissive.r = f4Color.m_fX * fEmissive;
    m_oD3d9Material.Emissive.g = f4Color.m_fY * fEmissive;
    m_oD3d9Material.Emissive.b = f4Color.m_fZ * fEmissive;
    m_oD3d9Material.Emissive.a = f4Color.m_fW * fEmissive;

    // Fills specular power
    m_oD3d9Material.Power = rData.GetSpecularPower();

    // Lighting is enabled if material color isn't black
    sxBool bLightingEnabled =       (f4Color.m_fX + f4Color.m_fY + f4Color.m_fZ + f4Color.m_fW) != 0.f
                                &&  (fAmbient + fDiffuse + fSpecular + fEmissive) != 0.f;

    // Specular is enabled if lighting is enabled and specular coefficient isn't 0.f
    m_bSpecualLightingEnabled = bLightingEnabled && (fSpecular != 0.f);
}

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9MaterialState::Dispatch(sxICommandData const& a_rData, sxIServerDevice& a_rDevice)
{
    // Fetch D3d9 device
    sxCD3d9Device& rDevice = (sxCD3d9Device&)a_rDevice;

    // Update shadow. Should be done in the server generic code rather than per state.
    rDevice.GetStateShadow().SetState(sxEStateType::eMaterial, rThis, a_rData);

    // Enable specular lighting
    sxD3d9Call(rDevice.GetD3dDevice().SetRenderState(D3DRS_SPECULARENABLE, m_bSpecualLightingEnabled));

    // Set the built material
    sxD3d9Call(rDevice.GetD3dDevice().SetMaterial(&m_oD3d9Material));
}
