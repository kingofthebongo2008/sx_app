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
#include "sxD3d9.h"

//-----------------------------------------------------------------------------------------------------------
// Include D3d9 libraries 

// Include base d3d9 lib
#pragma comment(lib, "d3d9.lib")

// Include base d3dx9 lib
#pragma comment(lib, "d3dx9_39.lib")


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCD3d9PresentParameter

//-----------------------------------------------------------------------------------------------------------
//
sxCD3d9PresentParameter::sxCD3d9PresentParameter(   IDirect3D9& a_rD3d9Object,
                                                    sxUInt a_uiWidth,
                                                    sxUInt a_uiHeight,
                                                    HWND a_hDisplayHandle)
{
    //-------------------------------------
    // Zero memory to be sure that all the parameters are initialized, even in the case of a future
    // modification in the D3DPRESENT_PARAMETERS structure 
    sxMemZero(m_oD3d9PresentParameter);

    //-------------------------------------
    // Back buffer is of a size of minimum 1 pixel
    m_oD3d9PresentParameter.BackBufferWidth = sxMax(1u, a_uiWidth);
    m_oD3d9PresentParameter.BackBufferHeight = sxMax(1u, a_uiHeight);
    m_oD3d9PresentParameter.BackBufferCount = 1;
    m_oD3d9PresentParameter.BackBufferFormat = D3DFMT_X8R8G8B8;

    //-------------------------------------
    // Automatically build the Z buffer
    m_oD3d9PresentParameter.EnableAutoDepthStencil = true;
    m_oD3d9PresentParameter.AutoDepthStencilFormat = D3DFMT_D24X8;
    
    //-------------------------------------
    // Multi sampling

    // Array of supported multi sampling types
    D3DMULTISAMPLE_TYPE aSupportedMultisampledType[] =
    {
        D3DMULTISAMPLE_4_SAMPLES,
        D3DMULTISAMPLE_2_SAMPLES,
    };

    // Selected type, NONE by default
    D3DMULTISAMPLE_TYPE oSelectedType = D3DMULTISAMPLE_NONE;

    // Iterate through all types and get the first supported
    sxForEachUInt(uiType, sxArraySize(aSupportedMultisampledType))
    {
        // Test if the type is supported
        if(     D3D_OK
            ==  a_rD3d9Object.CheckDeviceMultiSampleType(   D3DADAPTER_DEFAULT,
                                                            D3DDEVTYPE_HAL,
                                                            m_oD3d9PresentParameter.BackBufferFormat,
                                                            true,
                                                            aSupportedMultisampledType[uiType],
                                                            NULL))
        {
            // Got it
            oSelectedType = aSupportedMultisampledType[uiType];
            break;
        }
    }

    // Initialize using the selected type
    m_oD3d9PresentParameter.MultiSampleType = oSelectedType;
    m_oD3d9PresentParameter.MultiSampleQuality = 0;

    //-------------------------------------
    // Swapping
    m_oD3d9PresentParameter.SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_oD3d9PresentParameter.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    m_oD3d9PresentParameter.FullScreen_RefreshRateInHz = 0;
    m_oD3d9PresentParameter.Flags = 0;

    //-------------------------------------
    // Window
    m_oD3d9PresentParameter.Windowed = true;
    m_oD3d9PresentParameter.hDeviceWindow = a_hDisplayHandle;
}

