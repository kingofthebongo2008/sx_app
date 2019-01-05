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
#include "sxCD3d9VertexBufferState.h"
#include "sxDriver/Client/State/sxCVertexBufferState.h"
#include "sxDriver/Server/D3d9/sxCD3d9Device.h"

//-------------------------------------------------------------------------------------------------------
//
sxCD3d9VertexBufferState::sxCD3d9VertexBufferState() :
    m_uiVertexCount(0)
{
}

//-------------------------------------------------------------------------------------------------------
//
sxCD3d9VertexBufferState::~sxCD3d9VertexBufferState()
{
}

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9VertexBufferState::Build(sxICommandData const& a_rData, sxIServerDevice& a_rDevice)
{
    // Fetch vertex buffer state data
    sxCVertexBufferStateData const& rData = (sxCVertexBufferStateData const&)a_rData;

    //----------------------------
    // Reset current vertex buffer
    m_spVertexBuffer = NULL;
    m_uiVertexCount = 0;

    //---------------------------
    // Create a new vertex buffer

    // Fetch vertices from the state data
    sxCVertexBufferStateData::sxCVertices const& rVertices = rData.GetVertices();

    // Get vertex count from the data
    m_uiVertexCount = rVertices.size();

    // Don't need to build an empty vertex buffer state
    if(m_uiVertexCount == 0)
    {
        // Early out
        return;
    }

    // Compute vertex buffer lenght in bytes
    sxUInt uiByteLen = sizeof(sxCVertexBufferStateData::sxSVertex) * m_uiVertexCount;

    // Fetch device
    sxCD3d9Device& rDevice = (sxCD3d9Device&)a_rDevice;
    
    // Build the vertex buffer
    sxD3d9Call(rDevice.GetD3dDevice().CreateVertexBuffer(   uiByteLen,
                                                            D3DUSAGE_WRITEONLY,
                                                            D3DFVF_XYZ | D3DFVF_NORMAL,
                                                            D3DPOOL_MANAGED,
                                                            sxGetD3d9SmartModifier(m_spVertexBuffer),
                                                            NULL));

    //----------------------------------------------------
    // Fills the D3d9 vertex buffer with original vertices

    // Destination pointer
    void* pData;

    // Lock the vertex buffer
    sxD3d9Call(m_spVertexBuffer->Lock(  0,
                                        uiByteLen,
                                        &pData,
                                        0));

    // Copy vertices
    sxMemCopy(pData, &rVertices.front(), uiByteLen);

    // Unlock the vertex buffer
    sxD3d9Call(m_spVertexBuffer->Unlock());
}

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9VertexBufferState::Dispatch(sxICommandData const& a_rData, sxIServerDevice& a_rDevice)
{
    // Vertex buffer is already built so don't need to access the data again.

    // Fetch device
    sxCD3d9Device& rDevice = (sxCD3d9Device&)a_rDevice;

    // Update shadow. Should be done in the server generic code rather than per state.
    rDevice.GetStateShadow().SetState(sxEStateType::eVertexBuffer, rThis, a_rData);

    // Set the vertex buffer as the current one
    sxD3d9Call(rDevice.GetD3dDevice().SetStreamSource(  0,
                                                        m_spVertexBuffer,
                                                        0,
                                                        sizeof(sxCVertexBufferStateData::sxSVertex)));

    // Set the vertex format of this buffer
    sxD3d9Call(rDevice.GetD3dDevice().SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL));
}
