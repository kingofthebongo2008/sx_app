//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
//                                                                                                          |
//                              ShaderX7. Cross platform rendering thread. 2008.                            |
//              Guillaume Blanc. ELB (Etranges Libellules) http://www.elb-games.com/. Lyon, France.         |
//                                                                                                          |
//  This program is free software. It is distributed in the hope that it will be useful, but without any    |
//  warranty, without even the implied warranty of merchantability or fitness for a particular purpose.     |
//                                                                                                          |
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
#pragma once

#include "sxDriver/Server/Interface/sxIServerDevice.h"
#include "sxD3d9.h"
#include "sxID3d9ResetListener.h"
#include "sxCD3d9StateShadow.h"
#include "sxCD3d9TextRenderer.h"
#include "sxCD3d9GpuTimer.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Driver D3d9 server renderer
 class  sxCD3d9Device final : public sxIServerDevice, protected sxID3d9ResetWarner
{
public:

    //-------------------------------------------------------------------------------------------------------
    // Constructor
    sxCD3d9Device();

    //-------------------------------------------------------------------------------------------------------
    // Destructor
    virtual ~sxCD3d9Device();

    //-------------------------------------------------------------------------------------------------------
    // Get Direct3D interface
    IDirect3D9& GetD3DObject() const { return m_spD3dObject; }

    //-------------------------------------------------------------------------------------------------------
    // Get Direct3D Device interface
    IDirect3DDevice9& GetD3dDevice() const { return m_spD3dDevice; }

    //-------------------------------------------------------------------------------------------------------
    // Get State shadow
    sxCD3d9StateShadow& GetStateShadow() { return m_oStateShadow; }

    //-------------------------------------------------------------------------------------------------------
    // Get the unique device instance
    static sxCD3d9Device& GetInstance() { return (sxCD3d9Device&)sxID3d9ResetWarner::GetInstance(); }

    //-------------------------------------------------------------------------------------------------------
    // Set/Get device width
    void SetDeviceWidth(sxUInt a_uiValue);
    sxUInt GetDeviceWidth() const {return m_uiDeviceWidth;}

    //-------------------------------------------------------------------------------------------------------
    // Set/Get device height
    void SetDeviceHeight(sxUInt a_uiValue);
    sxUInt GetDeviceHeight() const {return m_uiDeviceHeight;}

    //-------------------------------------------------------------------------------------------------------
    // Begin D3d9 scene, can be called many times without requiring a end scene. Only the first call will be 
    // forwarded to D3d9
    void BeginScene();

    //-------------------------------------------------------------------------------------------------------
    // End D3d9 scene, must be called only once after one or multiple BeginScene()
    void EndScene();

    //-------------------------------------------------------------------------------------------------------
    // Set/Get the number of currently active lights.
    void ActivateLights(sxUInt a_uiLightCount);
    sxUInt GetActiveLightCount() const {return m_uiActiveLightCount;}

    //-------------------------------------------------------------------------------------------------------
    // Get the maximum active light count according to the hardware caps
    sxUInt GetMaxActiveLights() const {return m_uiMaxActiveLights;}

    //-------------------------------------------------------------------------------------------------------
    // Get the maximum primitive count according to the hardware caps
    sxUInt GetMaxPrimitiveCount() const {return m_uiMaxPrimitiveCount;}

    //-------------------------------------------------------------------------------------------------------
    // Get the maximum index count according to the hardware caps
    sxUInt GetMaxVertexIndex() const {return m_uiMaxVertexIndex;}

protected:

    //-------------------------------------------------------------------------------------------------------
    // Server renderer overloaded reset function.
    virtual sxBool Reset();

private:

    //-------------------------------------------------------------------------------------------------------
    // Reset the number of active lights
    void ResetActiveLightCount() {m_uiActiveLightCount = 0;}

    //-------------------------------------------------------------------------------------------------------
    // Dispaly overlayed text: statistics, controls
    void OverlayText();

    //-------------------------------------------------------------------------------------------------------
    // Attributes

    // D3d9 Object
    sxIDirect3D9Ptr m_spD3dObject;

    // D3d9 device
    sxIDirect3DDevice9Ptr m_spD3dDevice;

    // D3d9 state shadow
    sxCD3d9StateShadow m_oStateShadow;

    // Text renderer
    sxCD3d9TextRenderer m_oTextRenderer;

    // Gpu timer object
    sxCD3d9GpuTimer m_oGpuTimer;

    // Gpu fps counter
    sxCFpsCounter m_oGpuFpsCounter;

    // Handle used to create the device
    HWND m_hDeviceWindow;

    // Device width
    sxUInt m_uiDeviceWidth;

    // Device height
    sxUInt m_uiDeviceHeight;

    // Stores if the driver is inside BeginScene-EndScene calls
    sxBool m_bInsideBeginEndScene;

    // Number of currently active lights
    sxUInt m_uiActiveLightCount;

    // Caps: Maximum light count
    sxUInt m_uiMaxActiveLights;

    // Caps: Maximum primitive count per draw
    sxUInt m_uiMaxPrimitiveCount;

    // Caps: Maximum vertex index
    sxUInt m_uiMaxVertexIndex;
};
