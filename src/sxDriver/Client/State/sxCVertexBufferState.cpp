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
#include "sxCVertexBufferState.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Register sxCVertexBufferState default constructor
sxRegisterDefaultState(sxCVertexBufferState, sxEStateType::eVertexBuffer);

//-----------------------------------------------------------------------------------------------------------
//
void sxCVertexBufferStateData::Clear()
{
    m_vVertices.clear();
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCVertexBufferStateData::Copy(sxICommandData const& a_rData)
{
    // Fetch src data
    sxCVertexBufferStateData const& rData = (sxCVertexBufferStateData const&)a_rData;

    // Copy  vertices
    m_vVertices = rData.m_vVertices;
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCVertexBufferStateData::SetVertices(sxCVertices const& a_rVertices)
{
    // Copy indices vertices
    m_vVertices = a_rVertices;
}

//-----------------------------------------------------------------------------------------------------------
//
sxCVertexBufferStateData::sxCVertices& sxCVertexBufferStateData::GetVertices()
{
    return m_vVertices;
}

//-----------------------------------------------------------------------------------------------------------
//
sxCVertexBufferStateData::sxCVertices const& sxCVertexBufferStateData::GetVertices() const
{
    return m_vVertices;
}
