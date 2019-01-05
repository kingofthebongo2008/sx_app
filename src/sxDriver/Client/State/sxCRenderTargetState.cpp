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
#include "sxCRenderTargetState.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Register sxCRenderTargetState default constructor
sxRegisterDefaultState(sxCRenderTargetState, sxEStateType::eRenderTarget);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCRenderTargetStateData implementation

//-------------------------------------------------------------------------------------------------------
//
sxCRenderTargetStateData::sxCRenderTargetStateData() :
    m_uiWidth(0),
    m_uiHeight(0),
    m_f4Color(0, 0, 0, 0)
{
}

//-------------------------------------------------------------------------------------------------------
//
void sxCRenderTargetStateData::Clear()
{
    m_uiWidth = 0;
    m_uiHeight = 0;
    m_f4Color = sxCFloat4(0, 0, 0, 0);
}

//-------------------------------------------------------------------------------------------------------
//
void sxCRenderTargetStateData::Copy(sxICommandData const& a_rData)
{
    sxCRenderTargetStateData const& rData = (sxCRenderTargetStateData const&)a_rData;

    m_uiWidth = rData.m_uiWidth;
    m_uiHeight = rData.m_uiHeight;

    m_f4Color = rData.m_f4Color;
}
