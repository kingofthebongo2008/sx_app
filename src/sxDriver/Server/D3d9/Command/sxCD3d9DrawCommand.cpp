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
#include "sxCD3d9DrawCommand.h"
#include "sxDriver/Client/State/sxCLightState.h"
#include "sxDriver/Server/D3d9/sxCD3d9Device.h"
#include "sxDriver/Server/D3d9/State/sxCD3d9VertexBufferState.h"
#include "sxDriver/Server/D3d9/State/sxCD3d9TopologyState.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implement a drawing pass of the d3d9 server. Subdivides rendering into multiple passes according to the 
// light count. This object should be allocated locally as a helper object.
class sxCD3d9DrawPass : private sxINonCopyable
{
public:

    //-------------------------------------------------------------------------------------------------------
    // Constructor
    sxCD3d9DrawPass(sxCD3d9Device& a_rDevice);

    //-------------------------------------------------------------------------------------------------------
    // Begin a new pass
    void Begin();

    //-------------------------------------------------------------------------------------------------------
    // End of the current pass, returns true if no more pass is required
    sxBool End();

private:

    // The device we're working on
    sxCD3d9Device& m_rDevice;

    // The number of remaining lights required
    sxUInt m_uiRemainingLightCount;

    // The maximum number of lights per pass
    sxUInt m_uiLightPerPass;

    // The light vector to display
    sxCLightStateData::sxCLightVector const* m_pvLightVector;

    // Pass counter
    sxUInt m_uiPassNumber;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCD3d9DrawCommand

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9DrawCommand::Build( sxICommandData const& a_rData, sxIServerDevice& a_rDevice)
{
}

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9DrawCommand::Dispatch( sxICommandData const& a_rData, sxIServerDevice& a_rDevice)
{
    // Fetch D3d9 device
    sxCD3d9Device& rDevice = (sxCD3d9Device&)a_rDevice;

    // Fetch state shadow
    sxCD3d9StateShadow& rShadow = rDevice.GetStateShadow();

    //-------------------------------------------------------------------------------
    // Get vertex count from the current vertex buffer available in the server shadow

    // Fetch vertex buffer
    sxCD3d9VertexBufferState& rVertexBuffer =
        (sxCD3d9VertexBufferState&)rShadow.GetState(sxEStateType::eVertexBuffer);

    // Now get the required vertex count
    sxUInt const uiVertexCount = rVertexBuffer.GetVertexCount();

    // Skip rendering if vertex count is too high to be supported
    if(uiVertexCount > rDevice.GetMaxVertexIndex())
    {
        return;
    }

    //-------------------------------------------------------------------------------
    // Get primitive count from the current topology available in the server shadow

    // Fetch the topology
    sxCD3d9TopologyState& rTopology =
        (sxCD3d9TopologyState&)rShadow.GetState(sxEStateType::eTopology);

    //-------------------------------------------------------------------------------
    // Finally call the D3d9 Draw function, inise a BegineScene-EndScene pair

    // Begin a new scene (can be called multiple times)
    rDevice.BeginScene();

    // Create the pass object that'll help to set the lights per pass
    sxCD3d9DrawPass oPasser(rDevice);

    // Do required passes loops
    do
    {
        // Begin a new pass
        oPasser.Begin();

        //------------------------------
        // Draw current bound primitives
        // Might loop because of the "max primitive per draw" caps

        // Get the required primitive count
        sxUInt const uiPrimitiveCount = rTopology.GetPrimitiveCount();

        // Number of primitive that were rendered during this pass
        sxUInt uiRenderedPrimitiveCount = 0;

        do
        {
            // Compute the number of primitives that can be render this loop
            sxUInt const uiPrimitiveCountThisLoop = sxMin(  rDevice.GetMaxPrimitiveCount(),
                                                            uiPrimitiveCount - uiRenderedPrimitiveCount);

            // Render them
            sxD3d9Call(rDevice.GetD3dDevice().DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
                                                                    0,
                                                                    0,
                                                                    uiVertexCount,
                                                                    uiRenderedPrimitiveCount * 3,
                                                                    uiPrimitiveCountThisLoop));

            // Decrease remaing primitive count
            uiRenderedPrimitiveCount += uiPrimitiveCountThisLoop;
        }
        // Iterate until all primitives are rendered
        while(uiPrimitiveCount != uiRenderedPrimitiveCount);
    }
    // Iterate until the passer tells its the end
    while(oPasser.End() != true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCD3d9DrawPass implemetation

//-----------------------------------------------------------------------------------------------------------
//
sxCD3d9DrawPass::sxCD3d9DrawPass(sxCD3d9Device& a_rDevice) :
    m_rDevice(a_rDevice),
    m_uiRemainingLightCount(0),
    m_uiLightPerPass(0),
    m_pvLightVector(NULL),
    m_uiPassNumber(0)
{
    // Get the current light state in order to find the number of passes required
    sxCLightStateData const& rLightStateData =
        (sxCLightStateData const&)a_rDevice.GetStateShadow().GetStateData(sxEStateType::eLight);

    // Get the light vector
    m_pvLightVector = &rLightStateData.GetLights();

    // Get the number of lights from the data
    sxUInt const uiLightCount = m_pvLightVector->size();

    // Get the number of light per draw
    m_uiLightPerPass = a_rDevice.GetMaxActiveLights();

    // Compute the number of light passes
    m_uiRemainingLightCount = uiLightCount;
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCD3d9DrawPass::Begin()
{
    //-------------------------------------------------
    // Compute the number of light to display this pass
    sxUInt const uiPassLightCount = sxMin(m_uiRemainingLightCount, m_uiLightPerPass);

    // Decrement light counter
    m_uiRemainingLightCount -= uiPassLightCount;

    //-------------------------------------
    // Prepare and send lights for the pass

    // Allocate a d3d9 light
    D3DLIGHT9 oD3d9Light;
    sxMemZero(oD3d9Light);
    
    // Iterate through all the light of this pass
    sxForEachUInt(uiLight, uiPassLightCount)
    {
        // Fetch light
        sxCLight const& rLight = m_pvLightVector->at(m_uiRemainingLightCount + uiLight);

        // Initialize light common setting
        oD3d9Light.Type = D3DLIGHT_POINT;
        oD3d9Light.Range = 1000;

        // Get color from the data
        sxCFloat3 const& rf3Color = rLight.GetColor();

        // Fills ambient value
        sxFloat const fAmbient = rLight.GetAmbient();
        oD3d9Light.Ambient.r = rf3Color.m_fX * fAmbient;
        oD3d9Light.Ambient.g = rf3Color.m_fY * fAmbient;
        oD3d9Light.Ambient.b = rf3Color.m_fZ * fAmbient;
        oD3d9Light.Ambient.a = 1.f;

        // Fills diffuse value
        sxFloat const fDiffuse = rLight.GetDiffuse();
        oD3d9Light.Diffuse.r = rf3Color.m_fX * fDiffuse;
        oD3d9Light.Diffuse.g = rf3Color.m_fY * fDiffuse;
        oD3d9Light.Diffuse.b = rf3Color.m_fZ * fDiffuse;
        oD3d9Light.Diffuse.a = 1.f;

        // Fills specular value
        sxFloat const fSpecular = rLight.GetSpecular();
        oD3d9Light.Specular.r = rf3Color.m_fX * fSpecular;
        oD3d9Light.Specular.g = rf3Color.m_fY * fSpecular;
        oD3d9Light.Specular.b = rf3Color.m_fZ * fSpecular;
        oD3d9Light.Specular.a = 1.f;

        // Fills attenuation value
        oD3d9Light.Attenuation2 = rLight.GetAttenuation();

        // Get position from the data
        sxCFloat3 const& rf3Position = rLight.GetPosition();

        // Fills position
        oD3d9Light.Position.x = rf3Position.m_fX;
        oD3d9Light.Position.y = rf3Position.m_fY;
        oD3d9Light.Position.z = rf3Position.m_fZ;

        // Set it to the D3d9 device
        sxD3d9Call(m_rDevice.GetD3dDevice().SetLight(uiLight, &oD3d9Light));
    }

    // Activate the lights required for this pass
    m_rDevice.ActivateLights(uiPassLightCount);

    // The second pass initializes blending
    if(m_uiPassNumber == 1)
    {
        // Enable blending for all passses after the first
        sxD3d9Call(m_rDevice.GetD3dDevice().SetRenderState(D3DRS_ALPHABLENDENABLE, true));

        // Modify blending equation
        sxD3d9Call(m_rDevice.GetD3dDevice().SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE));

        // Accept only pixels that are already drawn
        sxD3d9Call(m_rDevice.GetD3dDevice().SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL));
    }

    // Increment pass number
    m_uiPassNumber++;
}

//-----------------------------------------------------------------------------------------------------------
//
sxBool sxCD3d9DrawPass::End()
{
    // Ends if not more light reamain
    sxBool bEnd = (m_uiRemainingLightCount == 0);

    // Reset render states on end
    if( bEnd == true
        && m_uiPassNumber > 1)
    {
        // Disable blending
        sxD3d9Call(m_rDevice.GetD3dDevice().SetRenderState(D3DRS_ALPHABLENDENABLE, false));

        // Use the standard z function
        sxD3d9Call(m_rDevice.GetD3dDevice().SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL));
    }

    return bEnd;
}
