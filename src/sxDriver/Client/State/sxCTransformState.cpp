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
#include "sxCTransformState.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Register sxCTransformState default constructor
sxRegisterDefaultState(sxCTransformState, sxEStateType::eTransform);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCTransformStateData implementation

//-------------------------------------------------------------------------------------------------------
//
sxCTransformStateData::sxCTransformStateData() :
    m_f3Right(1, 0, 0),
    m_f3Up(0, 1, 0),
    m_f3At(0, 0, 1),
    m_f3Pos(0, 0, 0)
{
}

//-------------------------------------------------------------------------------------------------------
//
void sxCTransformStateData::Clear()
{
    m_f3Right = sxCFloat3(1, 0, 0);
    m_f3Up = sxCFloat3(0, 1, 0);
    m_f3At = sxCFloat3(0, 0, 1);
    m_f3Pos = sxCFloat3(0, 0, 0);
}

//-------------------------------------------------------------------------------------------------------
//
void sxCTransformStateData::Copy(sxICommandData const& a_rData)
{
    sxCTransformStateData const& rData = (sxCTransformStateData const&)a_rData;

    m_f3Right = rData.m_f3Right;
    m_f3Up = rData.m_f3Up;
    m_f3At = rData.m_f3At;
    m_f3Pos = rData.m_f3Pos;
}
