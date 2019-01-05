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
#include "sxCD3d9LightState.h"
#include "sxDriver/Server/D3d9/sxCD3d9Device.h"
#include "sxDriver/Client/State/sxCLightState.h"

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9LightState::Build( sxICommandData const& a_rData, sxIServerDevice& a_rDevice)
{
}

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9LightState::Dispatch(sxICommandData const& a_rData, sxIServerDevice& a_rDevice)
{
    // Fetch D3d9 device
    sxCD3d9Device& rDevice = (sxCD3d9Device&)a_rDevice;

    // Update shadow. Should be done in the server generic code rather than per state.
    rDevice.GetStateShadow().SetState(sxEStateType::eLight, rThis, a_rData);
}
