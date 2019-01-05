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
#include "sxCD3d9TopologyState.h"
#include "sxDriver/Client/State/sxCTopologyState.h"
#include "sxDriver/Server/D3d9/sxCD3d9Device.h"

//-------------------------------------------------------------------------------------------------------
//
sxCD3d9TopologyState::sxCD3d9TopologyState() :
    m_uiIndexCount(0)
{
}

//-------------------------------------------------------------------------------------------------------
//
sxCD3d9TopologyState::~sxCD3d9TopologyState()
{
}

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9TopologyState::Build(sxICommandData const& a_rData, sxIServerDevice& a_rDevice)
{
    // Fetch topology buffer state data
    sxCTopologyStateData const& rData = (sxCTopologyStateData const&)a_rData;

    //----------------------------
    // Reset current index buffer
    m_spIndexBuffer = NULL;
    m_uiIndexCount = 0;

    //---------------------------
    // Create a new index buffer

    // Fetch vertices from the state data
    sxCTopologyStateData::sxCIndices const& rIndices = rData.GetIndices();

    // Get index count from the data
    m_uiIndexCount = rIndices.size();

    // Don't need to build an empty index buffer state
    if(m_uiIndexCount == 0)
    {
        // Early out
        return;
    }

    // Compute index buffer lenght in bytes
    sxUInt uiByteLen = sizeof(sxCTopologyStateData::sxIndex) * m_uiIndexCount;

    // Fetch device
    sxCD3d9Device& rDevice = (sxCD3d9Device&)a_rDevice;

    // Ensure data indexed format matches D3d9 format
    sxStaticAssert(sizeof(sxCTopologyStateData::sxIndex) == sizeof(sxUInt16), Invalid_index_format);
    
    // Build the index buffer
    sxD3d9Call(rDevice.GetD3dDevice().CreateIndexBuffer(    uiByteLen,
                                                            D3DUSAGE_WRITEONLY,
                                                            D3DFMT_INDEX16,
                                                            D3DPOOL_MANAGED,
                                                            sxGetD3d9SmartModifier(m_spIndexBuffer),
                                                            NULL));

    //----------------------------------------------------
    // Fills the D3d9 index buffer with original vertices

    // Destination pointer
    void* pData;

    // Lock the index buffer
    sxD3d9Call(m_spIndexBuffer->Lock(   0,
                                        uiByteLen,
                                        &pData,
                                        0));

    // Copy vertices
    sxMemCopy(pData, &rIndices.front(), uiByteLen);

    // Unlock the index buffer
    sxD3d9Call(m_spIndexBuffer->Unlock());
}

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9TopologyState::Dispatch(sxICommandData const& a_rData, sxIServerDevice& a_rDevice)
{
    // The index buffer is already built so don't need to access the data again.

    // Fetch device
    sxCD3d9Device& rDevice = (sxCD3d9Device&)a_rDevice;

    // Update shadow. Should be done in the server generic code rather than per state.
    rDevice.GetStateShadow().SetState(sxEStateType::eTopology, rThis, a_rData);
    
    // Set the index buffer as the current one
    sxD3d9Call(rDevice.GetD3dDevice().SetIndices(m_spIndexBuffer));
}
