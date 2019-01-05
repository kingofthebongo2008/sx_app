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
#include "sxCD3d9SwapCommand.h"
#include "sxDriver/Server/D3d9/sxCD3d9Device.h"

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9SwapCommand::Build( sxICommandData const& a_rData, sxIServerDevice& a_rDevice)
{
}

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9SwapCommand::Dispatch( sxICommandData const& a_rData, sxIServerDevice& a_rDevice)
{
    // Fetch D3d9 device
    sxCD3d9Device& rDevice = (sxCD3d9Device&)a_rDevice;

    // Ends the current scene
    rDevice.EndScene();

    // Present the back buffer
    sxD3d9CallHandleLost(rDevice, Present(NULL, NULL, NULL, NULL));
}
