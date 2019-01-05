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
#include "sxCTopologyState.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Register sxCVertexBufferState default constructor
sxRegisterDefaultState(sxCTopologyState, sxEStateType::eTopology);

//-----------------------------------------------------------------------------------------------------------
//
void sxCTopologyStateData::Clear()
{
    m_vIndices.clear();
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCTopologyStateData::Copy(sxICommandData const& a_rData)
{
    // Fetch src data
    sxCTopologyStateData const& rData = (sxCTopologyStateData const&)a_rData;

    // Copy  vertices
    m_vIndices = rData.m_vIndices;
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCTopologyStateData::SetIndices(sxCIndices const& a_vIndices)
{
    // Copy indices vector
    m_vIndices = a_vIndices;
}

//-----------------------------------------------------------------------------------------------------------
//
sxCTopologyStateData::sxCIndices& sxCTopologyStateData::GetIndices()
{
    return m_vIndices;
}

//-----------------------------------------------------------------------------------------------------------
//
sxCTopologyStateData::sxCIndices const& sxCTopologyStateData::GetIndices() const
{
    return m_vIndices;
}